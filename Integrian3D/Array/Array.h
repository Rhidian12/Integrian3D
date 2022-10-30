#pragma once

#include "../EngineConstants.h"
#include "../Iterator/Iterator.h"

#include <functional> /* std::function */

namespace Integrian3D
{
	struct Size_P final
	{
		uint64_t _Size;
	};
	struct Capacity_P final
	{
		uint64_t _Capacity;
	};

	Size_P operator""_size(const uint64_t i)
	{
		return Size_P{ i };
	}
	Capacity_P operator""_capacity(const uint64_t i)
	{
		return Capacity_P{ i };
	}

	/* [TODO]: Add allocator */
	template<typename T>
	class Array
	{
		inline constexpr static uint32_t SizeOfType{ sizeof(T) };
		using UnaryPred = std::function<bool(const T&)>;

	public:
		using It = Iterator<T, IteratorTag::RandomAccessIt>;
		using CIt = ConstIterator<T, IteratorTag::RandomAccessIt>;

#pragma region Ctors and Dtor
		constexpr Array() = default;
		/* [TODO]: add allocator to ctors */
		constexpr Array(const Size_P size)
			: Head{}
			, Tail{}
			, CurrentEnd{}
		{
			for (uint64_t i{}; i < size._Size; ++i)
			{
				EmplaceBack(T{});
			}
		}
		constexpr Array(const Size_P size, const T& val)
			: Head{}
			, Tail{}
			, CurrentEnd{}
		{
			for (uint64_t i{}; i < size._Size; ++i)
			{
				EmplaceBack(val);
			}
		}
		constexpr Array(const Capacity_P cap)
			: Head{}
			, Tail{}
			, CurrentEnd{}
		{
			Reserve(cap._Capacity);
		}
		constexpr Array(std::initializer_list<T> init)
			: Head{}
			, Tail{}
			, CurrentEnd{}
		{
			for (const T& elem : init)
			{
				EmplaceBack(elem);
			}
		}
		constexpr Array(It beg, It end)
			: Head{}
			, Tail{}
			, CurrentEnd{}
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
			if (Head)
			{
				DeleteData(Head, CurrentEnd);
				Release(Head);
			}

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
			if (Head)
			{
				DeleteData(Head, CurrentEnd);
				Release(Head);
			}

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
		constexpr void Add(const T& val)
		{
			EmplaceBack(val);
		}
		constexpr void Add(T&& val)
		{
			EmplaceBack(__MOVE(T, val));
		}

		constexpr void AddFront(const T& val)
		{
			EmplaceFront(val);
		}
		constexpr void AddFront(T&& val)
		{
			EmplaceFront(__MOVE(T, val));
		}

		constexpr void AddRange(std::initializer_list<T> elems)
		{
			for (const T& elem : elems)
			{
				EmplaceBack(elem);
			}
		}
		constexpr void AddRange(It beg, It end)
		{
			while (beg < end)
			{
				EmplaceBack(*beg);
				++beg;
			}
		}

		constexpr It Erase(const uint64_t index)
		{
			__ASSERT(index < Size() && "Array::Erase() > index is out of range");

			const uint64_t oldSize{ Size() };

			if (index == oldSize - 1)
			{
				Pop();

				return end();
			}
			else if (index == 0)
			{
				PopFront();

				return It{ Head };
			}
			else
			{
				(Head + index)->~T();
				MoveRange(Head + index + 1, CurrentEnd--, Head + index);

				return It{ Head + index };
			}
		}
		constexpr It Erase(It pos)
		{
			__ASSERT(pos != end() && "Array::Erase() > invalid iterator was passed as a parameter");

			return Erase(*pos);
		}
		constexpr It Erase(const T& val)
		{
			for (uint64_t i{}; i < Size(); ++i)
			{
				if (*(Head + i) == val)
				{
					return Erase(i);
					break;
				}
			}

			return end();
		}
		constexpr It Erase(const UnaryPred& pred)
		{
			It it{ Find(pred) };

			if (it != end())
			{
				return Erase(*it);
			}

			return end();
		}
		constexpr It Erase(UnaryPred&& pred)
		{
			It it{ Find(__MOVE(UnaryPred, pred)) };

			if (it != end())
			{
				return Erase(*it);
			}

			return end();
		}

		constexpr void EraseRange(const uint64_t start, const uint64_t count)
		{
			__ASSERT(start < Size() && "Array::EraseRange() > Start is out of range");

			EraseRange(It{ Head + start }, It{ Head + start + count });
		}
		constexpr void EraseRange(It beg, It endIt)
		{
			__ASSERT(beg != end() && "Array::EraseRange() > Cannot iterator past the end");

			if (endIt >= end())
			{
				endIt = It{ CurrentEnd - 1 };
			}

			for (; beg <= endIt; --endIt)
			{
				beg = Erase(beg);
			}
		}

		constexpr void Insert(const uint64_t index, const T& val)
		{
			Emplace(index, val);
		}
		constexpr void Insert(const uint64_t index, T&& val)
		{
			Emplace(index, __MOVE(T, val));
		}

		constexpr void Pop()
		{
			if (Size() == 0)
			{
				return;
			}

			(--CurrentEnd)->~T();
		}

		constexpr void PopFront()
		{
			if (Size() == 0)
			{
				return;
			}

			Head->~T();

			MoveRange(Head + 1, CurrentEnd--, Head);
		}

		constexpr void Clear()
		{
			DeleteData(Head, CurrentEnd);

			CurrentEnd = Head;
		}

		template<typename ... Ts>
		constexpr T& EmplaceBack(Ts&&... args)
		{
			/* if we point past our allocated memory we have an issue */
			if (!CurrentEnd || CurrentEnd >= Tail)
			{
				Reallocate();
			}

			return *(new (CurrentEnd++) T{ __FORWARD(Ts, args)... });
		}

		template<typename ... Ts>
		constexpr T& Emplace(const uint64_t index, Ts&&... args)
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

		template<typename ... Ts>
		constexpr T& EmplaceFront(Ts&&... args)
		{
			/* if we point past our allocated memory we have an issue */
			if (!CurrentEnd || CurrentEnd >= Tail)
			{
				Reallocate();
			}

			MoveRange(Head, CurrentEnd++, Head + 1);

			return *(new (Head) T{ __FORWARD(Ts, args)... });
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

		constexpr Array Select(const UnaryPred& pred) const
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
		constexpr Array Select(UnaryPred&& pred) const
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

		constexpr It Find(const T& val) const
		{
			for (uint64_t i{}; i < Size(); ++i)
			{
				if (*(Head + i) == val)
				{
					return It{ Head + i };
				}
			}

			return It{ CurrentEnd };
		}
		constexpr It Find(const UnaryPred& pred) const
		{
			for (uint64_t i{}; i < Size(); ++i)
			{
				if (pred(*(Head + i)))
				{
					return It{ Head + i };
				}
			}

			return It{ CurrentEnd };
		}
		constexpr It Find(UnaryPred&& pred) const
		{
			for (uint64_t i{}; i < Size(); ++i)
			{
				if (pred(*(Head + i)))
				{
					return It{ Head + i };
				}
			}

			return It{ CurrentEnd };
		}

		constexpr Array FindAll(const T& val) const
		{
			Array arr{};

			for (uint64_t i{}; i < Size(); ++i)
			{
				if (*(Head + i) == val)
				{
					arr.EmplaceBack(*(Head + i));
				}
			}

			return arr;
		}
		constexpr Array FindAll(const UnaryPred& pred) const
		{
			Array arr{};

			for (uint64_t i{}; i < Size(); ++i)
			{
				if (pred(*(Head + i)))
				{
					arr.EmplaceBack(*(Head + i));
				}
			}

			return arr;
		}
		constexpr Array FindAll(UnaryPred&& pred) const
		{
			Array arr{};

			for (uint64_t i{}; i < Size(); ++i)
			{
				if (pred(*(Head + i)))
				{
					arr.EmplaceBack(*(Head + i));
				}
			}

			return arr;
		}
#pragma endregion

#pragma region Iterators
		constexpr It begin() { return Head; }
		constexpr CIt begin() const { return Head; }

		constexpr It end() { return CurrentEnd; }
		constexpr CIt end() const { return CurrentEnd; }

		constexpr CIt cbegin() const { return Head; }
		constexpr CIt cend() const { return CurrentEnd; }
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