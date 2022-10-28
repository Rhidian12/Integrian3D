#pragma once

#include "../EngineConstants.h"

namespace Integrian3D
{
	/* [TODO]: Add allocator */
	class Array
	{
		using T = int;
		inline constexpr static uint32_t SizeOfType{ sizeof(T) };

	public:
		Array()
			: Head{}
			, Tail{}
			, LastElement{}
		{}
		~Array()
		{
			DeleteData(Head, Tail);
			Release(Head);
		}

#pragma region Adding and Removing Elements
		void Add(const T& val)
		{
			EmplaceBack(val);
		}
		void Add(T&& val)
		{
			EmplaceBack(__FORWARD(T, val));
		}

		void Pop()
		{
			if (Size() == 0)
			{
				return;
			}

			if constexpr (!std::is_trivially_destructible_v<T>)
			{
				LastElement->~T();
			}

			--LastElement;
		}

		void Clear()
		{
			DeleteData(Head, Tail);

			LastElement = nullptr;
		}

		template<typename ... Ts>
		T& EmplaceBack(Ts&&... args)
		{
			if (!LastElement || ++LastElement >= Tail)
			{
				Reallocate();
				++LastElement;
			}

			return *(new (LastElement) T{ __FORWARD(Ts, args)... });
		}
#pragma endregion

#pragma region Array Information
		__NODISCARD constexpr bool Empty() const
		{
			return Size() > 0;
		}

		__NODISCARD constexpr uint64_t Size() const
		{
			return LastElement - Head;
		}

		__NODISCARD constexpr uint64_t Capacity() const
		{
			return Tail - Head;
		}

		__NODISCARD constexpr uint64_t MaxSize() const
		{
			return std::numeric_limits<uint64_t>::max();
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

			return *(Head + 1);
		}
		const T& Front() const
		{
			__ASSERT(Size() > 0 && "Array::Front() > Array is empty");

			return *(Head + 1);
		}

		T& Back()
		{
			__ASSERT(Size() > 0 && "Array::Back() > Array is empty");

			return *LastElement;
		}
		const T& Back() const
		{
			__ASSERT(Size() > 0 && "Array::Back() > Array is empty");

			return *LastElement;
		}

		T& At(const uint64_t index)
		{
			__ASSERT((index < Size()) && "Array::At() > Index is out of range");

			return *(Head + index + 1);
		}
		const T& At(const uint64_t index) const
		{
			__ASSERT((index < Size()) && "Array::At() > Index is out of range");

			return *(Head + index + 1);
		}

		T& operator[](const uint64_t index)
		{
			return *(Head + index + 1);
		}
		const T& operator[](const uint64_t index) const
		{
			return *(Head + index + 1);
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
					new (Head + i) T{ __MOVE(T, *(Head + i)) };
				}
				else
				{
					new (Head + i) T{ *(Head + i) };
				}
			}

			LastElement = Head + oldSize;

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

			LastElement = Head + oldSize;

			DeleteData(oldHead, oldTail);
			Release(oldHead);
		}

		void DeleteData(T* head, T* const tail) const
		{
			if constexpr (!std::is_trivially_destructible_v<T>)
			{
				while (head <= tail)
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

		T* Head;
		T* Tail;
		T* LastElement;
	};
}