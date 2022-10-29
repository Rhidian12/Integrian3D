#pragma once

#include "../EngineConstants.h"

namespace Integrian3D
{
	/* [TODO]: Add allocator */
	template<typename T>
	class Array
	{
		inline constexpr static uint32_t SizeOfType{ sizeof(T) };

	public:
#pragma region Ctors and Dtor
		Array()
			: Head{}
			, Tail{}
			, CurrentEnd{}
		{}
		~Array()
		{
			DeleteData(Head, CurrentEnd);
			Release(Head);
		}
#pragma endregion

#pragma region Rule of 5
		Array(const Array& other) noexcept
			: Head{}
			, Tail{}
			, CurrentEnd{}
		{
			const uint64_t cap{ other.Capacity() };

			Head = static_cast<T*>(malloc(cap * SizeOfType));
			Tail = Head + cap;

			const uint64_t size{ other.Size() };

			for (uint64_t i{}; i < size; ++i)
			{
				new (Head + i) T{ *(other.Head + i) }; // dont allow moving 
			}

			CurrentEnd = Head + size;
		}
		Array(Array&& other) noexcept
			: Head{ __MOVE(T*, other.Head) }
			, Tail{ __MOVE(T*, other.Tail) }
			, CurrentEnd{ __MOVE(T*, other.CurrentEnd) }
		{
			other.Head = nullptr;
			other.Tail = nullptr;
			other.CurrentEnd = nullptr;
		}

		Array& operator=(const Array& other) noexcept
		{
			const uint64_t cap{ other.Capacity() };

			Head = static_cast<T*>(malloc(cap * SizeOfType));
			Tail = Head + cap;

			const uint64_t size{ other.Size() };

			for (uint64_t i{}; i < size; ++i)
			{
				new (Head + i) T{ *(other.Head + i) }; // dont allow moving 
			}

			CurrentEnd = Head + size;

			return *this;
		}
		Array& operator=(Array&& other) noexcept
		{
			Head = __MOVE(T*, other.Head);
			Tail = __MOVE(T*, other.Tail);
			CurrentEnd = __MOVE(T*, other.CurrentEnd);

			other.Head = nullptr;
			other.Tail = nullptr;
			other.CurrentEnd = nullptr;

			return *this;
		}
#pragma endregion

#pragma region Adding and Removing Elements
		void Add(const T& val)
		{
			EmplaceBack(val);
		}
		void Add(T&& val)
		{
			EmplaceBack(__MOVE(T, val));
		}

		void Insert(const uint64_t index, const T& val)
		{
			Emplace(index, val);
		}
		void Insert(const uint64_t index, T&& val)
		{
			Emplace(index, __MOVE(T, val));
		}

		void Pop()
		{
			if (Size() == 0)
			{
				return;
			}

			(--CurrentEnd)->~T();
		}

		void Clear()
		{
			DeleteData(Head, CurrentEnd);

			CurrentEnd = Head;
		}

		template<typename ... Ts>
		T& EmplaceBack(Ts&&... args)
		{
			/* if we point past our allocated memory we have an issue */
			if (!CurrentEnd || CurrentEnd >= Tail)
			{
				Reallocate();
			}

			return *(new (CurrentEnd++) T{ __FORWARD(Ts, args)... });
		}

		template<typename ... Ts>
		T& Emplace(const uint64_t index, Ts&&... args)
		{
			__ASSERT(index < Size() && "Array::Emplace() > index is out of range");

			const uint64_t oldSize{ Size() };

			if (oldSize + 1 > Capacity())
			{
				Reallocate();
			}

			if (oldSize == 0 || index == oldSize - 1)
			{
				return EmplaceBack(__FORWARD(Ts, args)...);
			}
			else
			{
				MoveRange(Head + index, CurrentEnd++ - 1, Head + index + 1);
				return *(new (Head + index) T{ __FORWARD(Ts, args)... });
			}
		}
#pragma endregion

#pragma region Array Information
		__NODISCARD constexpr bool Empty() const
		{
			return Size() == 0;
		}

		__NODISCARD constexpr uint64_t Size() const
		{
			return CurrentEnd - Head;
		}

		__NODISCARD constexpr uint64_t Capacity() const
		{
			return Tail - Head;
		}

		__NODISCARD constexpr uint64_t MaxSize() const
		{
			return std::numeric_limits<uint64_t>::max();
		}

		__NODISCARD constexpr bool operator==(const Array& other) const
		{
			const uint64_t size{ Size() };

			if (size != other.Size())
			{
				return false;
			}

			for (uint64_t i{}; i < size; ++i)
			{
				if (*(Head + i) != *(other.Head + i))
				{
					return false;
				}
			}

			return true;
		}

		__NODISCARD constexpr bool operator!=(const Array& other) const
		{
			return !(*this == other);
		}
#pragma endregion

#pragma region Manipulating Array
		void Reserve(const uint64_t newCap)
		{
			if (newCap > Capacity())
			{
				if (newCap < MaxSize())
				{
					ReallocateExactly(newCap);
				}
			}
		}

		void Resize(const uint64_t newSize)
		{
			static_assert(std::is_default_constructible_v<T>, "Array::Resize() > T is not default constructable!");

			Resize(newSize, T{});
		}
		template<typename U>
		void Resize(const uint64_t newSize, U&& val)
		{
			static_assert(std::is_default_constructible_v<U>, "Array::Resize() > T is not default constructable!");
			static_assert(std::is_same_v<T, U>, "Array::Resize() > U and T must be the same!");

			const uint64_t oldSize{ Size() };

			if (newSize > oldSize)
			{
				const uint64_t diff{ newSize - oldSize };

				for (uint64_t i{}; i < diff; ++i)
				{
					EmplaceBack(__FORWARD(U, val));
				}

				return;
			}

			if (newSize < oldSize)
			{
				const uint64_t diff{ oldSize - newSize };

				for (uint64_t i{}; i < diff; ++i)
				{
					Pop();
				}

				return;
			}
		}

		void ShrinkToFit()
		{
			if (Size() == Capacity())
				return;

			ReallocateExactly(Size());
		}
#pragma endregion

#pragma region Accessing Elements
		T& Front()
		{
			__ASSERT(Size() > 0 && "Array::Front() > Array is empty");

			return *Head;
		}
		const T& Front() const
		{
			__ASSERT(Size() > 0 && "Array::Front() > Array is empty");

			return *Head;
		}

		T& Back()
		{
			__ASSERT(Size() > 0 && "Array::Back() > Array is empty");

			return *(CurrentEnd - 1);
		}
		const T& Back() const
		{
			__ASSERT(Size() > 0 && "Array::Back() > Array is empty");

			return *(CurrentEnd - 1);
		}

		T& At(const uint64_t index)
		{
			__ASSERT((index < Size()) && "Array::At() > Index is out of range");

			return *(Head + index);
		}
		const T& At(const uint64_t index) const
		{
			__ASSERT((index < Size()) && "Array::At() > Index is out of range");

			return *(Head + index);
		}

		T& operator[](const uint64_t index)
		{
			return *(Head + index);
		}
		const T& operator[](const uint64_t index) const
		{
			return *(Head + index);
		}

		T* const Data()
		{
			return Head;
		}
		const T* const Data() const
		{
			return Head;
		}
#pragma endregion

	private:
#pragma region Internal Helpers
		void Reallocate()
		{
			const uint64_t oldSize{ Size() };
			const uint64_t newCap{ CalculateNewCapacity(oldSize + 1) };

			T* oldHead{ Head };
			T* oldTail{ Tail };

			Head = static_cast<T*>(malloc(SizeOfType * newCap));
			Tail = Head + newCap;

			for (uint64_t i{}; i < oldSize; ++i)
			{
				if constexpr (std::is_move_assignable_v<T>)
				{
					new (Head + i) T{ __MOVE(T, *(oldHead + i)) };
				}
				else
				{
					new (Head + i) T{ *(oldHead + i) };
				}
			}

			CurrentEnd = Head + oldSize;

			DeleteData(oldHead, oldTail);
			Release(oldHead);
		}
		void ReallocateExactly(const uint64_t newCap)
		{
			const uint64_t oldSize{ Size() };

			T* oldHead{ Head };
			T* oldTail{ Tail };

			Head = static_cast<T*>(malloc(SizeOfType * newCap));
			Tail = Head + newCap;

			for (uint64_t i{}; i < oldSize; ++i)
			{
				if constexpr (std::is_move_assignable_v<T>)
				{
					new (Head + i) T{ __MOVE(T, *(Head + i)) };
				}
				else
				{
					new (Head + i) T{ *(Head + i) };
				}
			}

			CurrentEnd = Head + oldSize;

			DeleteData(oldHead, oldTail);
			Release(oldHead);
		}

		void DeleteData(T* head, T* const tail) const
		{
			if constexpr (!std::is_trivially_destructible_v<T>)
			{
				while (head < tail)
				{
					head->~T();
					++head;
				}
			}
		}

		void Release(T* head) const
		{
			free(head);
		}

		__NODISCARD constexpr uint64_t CalculateNewCapacity(const uint64_t min) const
		{
			const uint64_t oldCap{ Capacity() };
			const uint64_t maxCap{ MaxSize() };

			if (oldCap > maxCap - oldCap / 2u)
			{
				return maxCap;
			}

			const uint64_t newCap{ oldCap + oldCap / 2u };

			// If our growth is insufficient, return just the bare minimum
			if (newCap < min)
			{
				return min;
			}

			return newCap;
		}

		void MoveRange(T* head, T* end, T* newHead) const
		{
			__ASSERT(end > head);

			for (uint64_t i{}; i < static_cast<uint64_t>(end - head); ++i)
			{
				if constexpr (std::is_move_assignable_v<T>)
				{
					new (newHead + i) T{ __MOVE(T, *(head + i)) };
				}
				else
				{
					new (newHead + i) T{ *(head + i) };
				}
			}
		}
#pragma endregion

		T* Head;
		T* Tail;
		T* CurrentEnd /* points PAST the last element */;
	};
}