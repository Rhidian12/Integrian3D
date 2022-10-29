#pragma once

#include "../EngineConstants.h"
#include "../Iterator/Iterator.h"

#include <functional> /* std::function */

namespace Integrian3D
{
	struct Size final
	{
		uint64_t _Size;
	};
	struct Capacity final
	{
		uint64_t _Capacity;
	};

	Size operator""_size(const uint64_t i)
	{
		return Size{ i };
	}
	Capacity operator""_capacity(const uint64_t i)
	{
		return Capacity{ i };
	}

	/* [TODO]: Add allocator */
	template<typename T>
	class Array
	{
		inline constexpr static uint32_t SizeOfType{ sizeof(T) };
		using UnarySelectPred = std::function<bool(const T&)>;

	public:
#pragma region Ctors and Dtor
		constexpr Array() = default;
		/* [TODO]: add allocator to ctors */
		constexpr Array(const Size size)
		{
			for (uint64_t i{}; i < size._Size; ++i)
			{
				EmplaceBack(T{});
			}
		}
		constexpr Array(const Size size, const T& val)
		{
			for (uint64_t i{}; i < size._Size; ++i)
			{
				EmplaceBack(val);
			}
		}
		constexpr Array(const Capacity cap)
		{
			Reserve(cap._Capacity);
		}
		constexpr Array(std::initializer_list<T> init)
		{
			for (const T& elem : init)
			{
				EmplaceBack(elem);
			}
		}
		constexpr Array(Iterator<T, IteratorTag::RandomAccessIt> beg, Iterator<T, IteratorTag::RandomAccessIt> end)
		{
			while (beg < end)
			{
				EmplaceBack(*beg);
				++beg;
			}
		}

		constexpr ~Array()
		{
			DeleteData(Head, CurrentEnd);
			Release(Head);
		}
#pragma endregion

#pragma region Rule of 5
		constexpr Array(const Array& other) noexcept
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
		constexpr Array(Array&& other) noexcept
			: Head{ __MOVE(T*, other.Head) }
			, Tail{ __MOVE(T*, other.Tail) }
			, CurrentEnd{ __MOVE(T*, other.CurrentEnd) }
		{
			other.Head = nullptr;
			other.Tail = nullptr;
			other.CurrentEnd = nullptr;
		}

		constexpr Array& operator=(const Array& other) noexcept
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
		constexpr Array& operator=(Array&& other) noexcept
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
		constexpr void Reserve(const uint64_t newCap)
		{
			if (newCap > Capacity())
			{
				if (newCap < MaxSize())
				{
					ReallocateExactly(newCap);
				}
			}
		}

		constexpr void Resize(const uint64_t newSize)
		{
			static_assert(std::is_default_constructible_v<T>, "Array::Resize() > T is not default constructable!");

			Resize(newSize, T{});
		}
		template<typename U>
		constexpr void Resize(const uint64_t newSize, U&& val)
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

		constexpr void ShrinkToFit()
		{
			if (Size() == Capacity())
				return;

			ReallocateExactly(Size());
		}

		constexpr Array Select(const UnarySelectPred& pred) const
		{
			const uint64_t size{ Size() };

			Array arr{};
			arr.Reserve(size); // Change this when a constructor has been added for capacity

			for (uint64_t i{}; i < size; ++i)
			{
				const T* const elem{ Head + i };

				if (pred(*elem))
				{
					arr.Add(*elem);
				}
			}

			return arr;
		}
		constexpr Array Select(UnarySelectPred&& pred) const
		{
			const uint64_t size{ Size() };

			Array arr{};
			arr.Reserve(size); // Change this when a constructor has been added for capacity

			for (uint64_t i{}; i < size; ++i)
			{
				const T* const elem{ Head + i };

				if (pred(*elem))
				{
					arr.Add(*elem);
				}
			}

			return arr;
		}
#pragma endregion

#pragma region Accessing Elements
		constexpr T& Front()
		{
			__ASSERT(Size() > 0 && "Array::Front() > Array is empty");

			return *Head;
		}
		constexpr const T& Front() const
		{
			__ASSERT(Size() > 0 && "Array::Front() > Array is empty");

			return *Head;
		}

		constexpr T& Back()
		{
			__ASSERT(Size() > 0 && "Array::Back() > Array is empty");

			return *(CurrentEnd - 1);
		}
		constexpr const T& Back() const
		{
			__ASSERT(Size() > 0 && "Array::Back() > Array is empty");

			return *(CurrentEnd - 1);
		}

		constexpr T& At(const uint64_t index)
		{
			__ASSERT((index < Size()) && "Array::At() > Index is out of range");

			return *(Head + index);
		}
		constexpr const T& At(const uint64_t index) const
		{
			__ASSERT((index < Size()) && "Array::At() > Index is out of range");

			return *(Head + index);
		}

		constexpr T& operator[](const uint64_t index)
		{
			return *(Head + index);
		}
		constexpr const T& operator[](const uint64_t index) const
		{
			return *(Head + index);
		}

		constexpr T* const Data()
		{
			return Head;
		}
		constexpr const T* const Data() const
		{
			return Head;
		}
#pragma endregion

#pragma region Iterators
		Iterator<T, IteratorTag::RandomAccessIt> begin() { return Head; }
		ConstIterator<T, IteratorTag::RandomAccessIt> begin() const { return Head; }

		Iterator<T, IteratorTag::RandomAccessIt> end() { return Head + Size(); }
		ConstIterator<T, IteratorTag::RandomAccessIt> end() const { return Head + Size(); }

		ConstIterator<T, IteratorTag::RandomAccessIt> cbegin() const { return Head; }
		ConstIterator<T, IteratorTag::RandomAccessIt> cend() const { return Head + Size(); }
#pragma endregion

	private:
#pragma region Internal Helpers
		constexpr void Reallocate()
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
		constexpr void ReallocateExactly(const uint64_t newCap)
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

		constexpr void DeleteData(T* head, T* const tail) const
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

		constexpr void Release(T* head) const
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

		constexpr void MoveRange(T* head, T* end, T* newHead) const
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