#pragma once

#include "Iterator/Iterator.h"
#include "DebugUtility/DebugUtility.h"
#include "Macros/Macros.h"
#include "Types/Types.h"

#include <functional> /* std::function */

namespace Integrian3D
{
	#define TO_INT32(Expr) static_cast<int32>(Expr)

	template<typename T>
	class Array
	{
		using UnaryPred = std::function<bool(const T&)>;
		using BinaryPred = std::function<bool(const T&, const T&)>;

		static_assert(std::is_move_constructible_v<T> || std::is_copy_constructible_v<T>, "TArray > Type must be move or copy constructible");

	public:
		using It = Iterator<T, IteratorTag::RandomAccessIt>;
		using CIt = ConstIterator<T, IteratorTag::RandomAccessIt>;

	#pragma region Ctors and Dtor
		constexpr Array()
			: m_pHead{}
			, m_pTail{}
			, m_pCurrentEnd{}
		{}
		constexpr Array(const Size_P size)
			: m_pHead{}
			, m_pTail{}
			, m_pCurrentEnd{}
		{
			for (int32 i{}; i < size._Size; ++i)
				EmplaceBack(T{});
		}
		constexpr Array(const Size_P size, const T& val)
			: m_pHead{}
			, m_pTail{}
			, m_pCurrentEnd{}
		{
			for (int32 i{}; i < size._Size; ++i)
				EmplaceBack(val);
		}
		constexpr Array(const Capacity_P cap)
			: m_pHead{}
			, m_pTail{}
			, m_pCurrentEnd{}
		{
			Reserve(cap._Capacity);
		}
		constexpr Array(std::initializer_list<T> init)
			: m_pHead{}
			, m_pTail{}
			, m_pCurrentEnd{}
		{
			for (const T& elem : init)
			{
				EmplaceBack(elem);
			}
		}
		constexpr Array(It beg, It end)
			: m_pHead{}
			, m_pTail{}
			, m_pCurrentEnd{}
		{
			for (; beg != end; ++beg)
			{
				EmplaceBack(*beg);
			}
		}

		constexpr ~Array()
		{
			DeleteData(m_pHead, m_pCurrentEnd);
			Release(m_pHead);
		}
	#pragma endregion

	#pragma region Rule of 5
		constexpr Array(const Array& other) noexcept
			: m_pHead{}
			, m_pTail{}
			, m_pCurrentEnd{}
		{
			static_assert(std::is_copy_constructible_v<T>, "TArray > Type must be copy constructible");

			const int32 cap{ other.Capacity() };
			if (cap > 0u)
			{
				m_pHead = static_cast<T*>(malloc(cap * sizeof(T)));
				m_pTail = m_pHead + cap;
				m_pCurrentEnd = m_pHead + other.Size();

				CopyData<false>(m_pHead, other.m_pHead, other.Size());
			}
		}
		constexpr Array(Array&& other) noexcept
			: m_pHead{ I_MOVE(other.m_pHead) }
			, m_pTail{ I_MOVE(other.m_pTail) }
			, m_pCurrentEnd{ I_MOVE(other.m_pCurrentEnd) }
		{
			other.m_pHead = nullptr;
			other.m_pTail = nullptr;
			other.m_pCurrentEnd = nullptr;
		}

		constexpr Array& operator=(const Array& other) noexcept
		{
			static_assert(std::is_copy_constructible_v<T>, "TArray > Type must be copy constructible");

			if (m_pHead)
			{
				DeleteData(m_pHead, m_pCurrentEnd);
				Release(m_pHead);
			}

			m_pHead = nullptr;
			m_pTail = nullptr;
			m_pCurrentEnd = nullptr;

			const int32 cap{ other.Capacity() };
			if (cap > 0u)
			{
				m_pHead = static_cast<T*>(malloc(cap * sizeof(T)));
				m_pTail = m_pHead + cap;
				m_pCurrentEnd = m_pHead + other.Size();

				CopyData<false>(m_pHead, other.m_pHead, other.Size());
			}

			return *this;
		}
		constexpr Array& operator=(Array&& other) noexcept
		{
			if (m_pHead)
			{
				DeleteData(m_pHead, m_pCurrentEnd);
				Release(m_pHead);
			}

			m_pHead = I_MOVE(other.m_pHead);
			m_pTail = I_MOVE(other.m_pTail);
			m_pCurrentEnd = I_MOVE(other.m_pCurrentEnd);

			other.m_pHead = nullptr;
			other.m_pTail = nullptr;
			other.m_pCurrentEnd = nullptr;

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
			EmplaceBack(I_MOVE(val));
		}

		constexpr void AddUnique(const T& Val)
		{
			if (Find(Val) == end())
			{
				EmplaceBack(Val);
			}
		}

		constexpr void AddFront(const T& val)
		{
			EmplaceFront(val);
		}
		constexpr void AddFront(T&& val)
		{
			EmplaceFront(I_MOVE(val));
		}

		constexpr void AddRange(std::initializer_list<T> elems)
		{
			for (const T& elem : elems)
			{
				EmplaceBack(elem);
			}
		}
		constexpr void AddRange(It beg, const It end)
		{
			for (; beg != end; ++beg)
			{
				EmplaceBack(*beg);
			}
		}
		constexpr void AddRange(CIt beg, const CIt end)
		{
			for (; beg != end; ++beg)
			{
				EmplaceBack(*beg);
			}
		}
		constexpr void AddRange(T* pArr, const int32 n)
		{
			__ASSERT(pArr != nullptr, "Array::AddRange > Passed array cannot be null");

			for (int32 i{}; i < n; ++i)
			{
				EmplaceBack(pArr[i]);
			}
		}
		constexpr void AddRange(const Array& Other)
		{
			for (const T& Element : Other)
			{
				EmplaceBack(Element);
			}
		}

		constexpr It EraseByIndex(const int32 index)
		{
			__ASSERT(index < Size(), "Array::Erase() > index {} is out of range", index);

			const int32 oldSize{ Size() };

			if (index == oldSize - 1)
			{
				Pop();

				return end();
			}
			else if (index == 0)
			{
				PopFront();

				return begin();
			}
			else
			{
				(m_pHead + index)->~T();
				MoveRange(m_pHead + index + 1, m_pCurrentEnd--, m_pHead + index);

				return It{ m_pHead + index };
			}
		}

		constexpr It Erase(It pos)
		{
			__ASSERT(pos != end(), "Array::Erase() > Pos cannot be equal to end()");

			return EraseByIndex(TO_INT32(pos.Data() - begin().Data()));
		}
		constexpr It Erase(const T& val)
		{
			const int32 size{ Size() };
			for (int32 i{}; i < size; ++i)
			{
				if (*(m_pHead + i) == val)
				{
					return EraseByIndex(i);
				}
			}

			return end();
		}
		constexpr It Erase(const UnaryPred& pred)
		{
			It it{ Find(pred) };

			if (it != end())
			{
				return EraseByIndex(TO_INT32(it.Data() - begin().Data()));
			}

			return end();
		}

		constexpr void EraseAll(const UnaryPred& pred)
		{
			for (It beg{ begin() }; beg != end();)
			{
				if (pred(*beg))
				{
					beg = Erase(beg);
				}
				else
				{
					++beg;
				}
			}
		}
		constexpr void EraseAll(const T& val)
		{
			for (It beg{ begin() }; beg != end();)
			{
				if (*beg == val)
				{
					beg = Erase(beg);
				}
				else
				{
					++beg;
				}
			}
		}

		constexpr void EraseRange(const int32 start, const int32 count)
		{
			__ASSERT(start < Size(), "Array::EraseRange() > Start cannot be greater than the Array size");

			EraseRange(It{ m_pHead + start }, It{ m_pHead + start + count });
		}
		constexpr void EraseRange(It beg, It endIt)
		{
			__ASSERT(beg < end(), "Array::EraseRange() > Begin cannot be greater or equal to End");

			if (endIt >= end())
			{
				endIt = It{ m_pCurrentEnd - 1 };
			}

			for (; beg <= endIt; --endIt)
			{
				beg = Erase(beg);
			}
		}

		constexpr void Insert(const int32 index, const T& val)
		{
			Emplace(index, val);
		}
		constexpr void Insert(const int32 index, T&& val)
		{
			Emplace(index, I_MOVE(val));
		}

		constexpr void Pop()
		{
			if (Size() == 0)
			{
				return;
			}

			(--m_pCurrentEnd)->~T();
		}

		constexpr void PopFront()
		{
			if (Size() == 0)
			{
				return;
			}

			m_pHead->~T();

			MoveRange(m_pHead + 1, m_pCurrentEnd--, m_pHead);
		}

		constexpr void Clear()
		{
			DeleteData(m_pHead, m_pCurrentEnd);

			m_pCurrentEnd = m_pHead;
		}

		template<typename ... Ts>
		constexpr T& EmplaceBack(Ts&&... args)
		{
			/* if we point past our allocated memory we have an issue */
			if (!m_pCurrentEnd || m_pCurrentEnd >= m_pTail)
			{
				Reallocate();
			}

			return *(new (m_pCurrentEnd++) T{ __FORWARD(args)... });
		}

		template<typename ... Ts>
		constexpr T& Emplace(const int32 index, Ts&&... args)
		{
			__ASSERT(index <= Size(), "Array::Emplace() > Index cannot be greater or equal to the Array size");

			const int32 oldSize{ Size() };

			if (oldSize + 1 > Capacity())
			{
				Reallocate();
			}

			if (index == 0)
			{
				return EmplaceFront(__FORWARD(args)...);
			}
			else if (oldSize == 0 || index == oldSize || index == oldSize - 1)
			{
				return EmplaceBack(__FORWARD(args)...);
			}
			else
			{
				MoveRange(m_pHead + index, m_pCurrentEnd++ - 1, m_pHead + index + 1);
				return *(new (m_pHead + index) T{ __FORWARD(args)... });
			}
		}

		template<typename ... Ts>
		constexpr T& EmplaceFront(Ts&&... args)
		{
			/* if we point past our allocated memory we have an issue */
			if (!m_pCurrentEnd || m_pCurrentEnd >= m_pTail)
			{
				Reallocate();
			}

			MoveRange(m_pHead, m_pCurrentEnd++, m_pHead + 1);

			return *(new (m_pHead) T{ __FORWARD(args)... });
		}
	#pragma endregion

	#pragma region Array Information
		__NODISCARD constexpr bool Empty() const
		{
			return Size() == 0;
		}

		__NODISCARD constexpr int32 Size() const
		{
			return TO_INT32(m_pCurrentEnd - m_pHead);
		}

		__NODISCARD constexpr int32 Capacity() const
		{
			return TO_INT32(m_pTail - m_pHead);
		}

		__NODISCARD constexpr int32 MaxSize() const
		{
			return std::numeric_limits<int32>::max();
		}

		__NODISCARD constexpr bool operator==(const Array& other) const
		{
			const int32 size{ Size() };

			if (size != other.Size())
			{
				return false;
			}

			for (int32 i{}; i < size; ++i)
			{
				if (*(m_pHead + i) != *(other.m_pHead + i))
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
		constexpr void Reserve(const int32 newCap)
		{
			if (newCap > Capacity())
			{
				if (newCap < MaxSize())
				{
					ReallocateExactly(newCap);
				}
			}
		}

		constexpr void Resize(const int32 newSize)
		{
			static_assert(std::is_default_constructible_v<T>, "Array::Resize() > T is not default constructable!");

			Resize(newSize, T{});
		}
		template<typename U>
		constexpr void Resize(const int32 newSize, U&& val)
		{
			static_assert(std::is_default_constructible_v<U>, "Array::Resize() > T is not default constructable!");
			static_assert(std::is_same_v<T, U>, "Array::Resize() > U and T must be the same!");

			const int32 oldSize{ Size() };

			if (newSize > oldSize)
			{
				const int32 diff{ newSize - oldSize };

				for (int32 i{}; i < diff; ++i)
				{
					EmplaceBack(__FORWARD(val));
				}

				return;
			}

			if (newSize < oldSize)
			{
				const int32 diff{ oldSize - newSize };

				for (int32 i{}; i < diff; ++i)
				{
					Pop();
				}

				return;
			}
		}

		constexpr void ShrinkToFit()
		{
			if (Size() == Capacity())
			{
				return;
			}

			ReallocateExactly(Size());
		}

		constexpr Array Select(const UnaryPred& pred) const
		{
			const int32 size{ Size() };

			Array arr{ Capacity_P{ size } };

			for (int32 i{}; i < size; ++i)
			{
				const T* const elem{ m_pHead + i };

				if (pred(*elem))
				{
					arr.Add(*elem);
				}
			}

			return arr;
		}
		constexpr Array Select(UnaryPred&& pred) const
		{
			const int32 size{ Size() };

			Array arr{ Capacity_P{ size } };

			for (int32 i{}; i < size; ++i)
			{
				const T* const elem{ m_pHead + i };

				if (pred(*elem))
				{
					arr.Add(*elem);
				}
			}

			return arr;
		}

		constexpr Array SubArray(const int32 startIndex, const int32 count = std::numeric_limits<int32>::max()) const
		{
			Array arr{};

			const int32 size{ Size() };
			for (int32 i{ startIndex }; i < count; ++i)
			{
				if (i >= size)
				{
					break;
				}

				arr.Add(*(m_pHead + i));
			}

			return arr;
		}

		constexpr void Sort() const
		{
			Sort([](const T& a, const T& b)->bool
				{
					return a < b;
				});
		}
		constexpr void Sort(const BinaryPred& pred) const
		{
			if (!m_pHead) return;

			const int32 size{ Size() };

			if (size < 64u)
			{
				InsertionSort(pred);
			}
			else
			{
				MergeSort(0u, size - 1u, pred);
			}
		}
	#pragma endregion

	#pragma region Accessing Elements
		constexpr T& Front()
		{
			__CHECK(Size() > 0);

			return *m_pHead;
		}
		constexpr const T& Front() const
		{
			__CHECK(Size() > 0);

			return *m_pHead;
		}

		constexpr T& Back()
		{
			__CHECK(Size() > 0);

			return *(m_pCurrentEnd - 1);
		}
		constexpr const T& Back() const
		{
			__CHECK(Size() > 0);

			return *(m_pCurrentEnd - 1);
		}

		constexpr T& At(const int32 index)
		{
			__ASSERT(index < Size(), "Array::At() > Index is out of range");

			return *(m_pHead + index);
		}
		constexpr const T& At(const int32 index) const
		{
			__ASSERT(index < Size(), "Array::At() > Index is out of range");

			return *(m_pHead + index);
		}

		constexpr T& operator[](const int32 index)
		{
			return *(m_pHead + index);
		}
		constexpr const T& operator[](const int32 index) const
		{
			return *(m_pHead + index);
		}

		constexpr T* const Data()
		{
			return m_pHead;
		}
		constexpr const T* const Data() const
		{
			return m_pHead;
		}

		constexpr bool Contains(const T& val) const
		{
			return Find(val) != cend();
		}

		constexpr bool Contains(const UnaryPred& pred) const
		{
			return Find(pred) != cend();
		}

		constexpr It Find(const T& val)
		{
			const int32 size{ Size() };
			for (int32 i{}; i < size; ++i)
			{
				if (*(m_pHead + i) == val)
				{
					return It{ m_pHead + i };
				}
			}

			return It{ m_pCurrentEnd };
		}
		constexpr CIt Find(const T& val) const
		{
			const int32 size{ Size() };
			for (int32 i{}; i < size; ++i)
			{
				if (*(m_pHead + i) == val)
				{
					return CIt{ m_pHead + i };
				}
			}

			return CIt{ m_pCurrentEnd };
		}

		constexpr It Find(const UnaryPred& pred)
		{
			const int32 size{ Size() };
			for (int32 i{}; i < size; ++i)
			{
				if (pred(*(m_pHead + i)))
				{
					return It{ m_pHead + i };
				}
			}

			return It{ m_pCurrentEnd };
		}
		constexpr CIt Find(const UnaryPred& pred) const
		{
			const int32 size{ Size() };
			for (int32 i{}; i < size; ++i)
			{
				if (pred(*(m_pHead + i)))
				{
					return CIt{ m_pHead + i };
				}
			}

			return CIt{ m_pCurrentEnd };
		}

		constexpr Array FindAll(const T& val) const
		{
			Array arr{};

			const int32 size{ Size() };
			for (int32 i{}; i < size; ++i)
			{
				if (*(m_pHead + i) == val)
				{
					arr.EmplaceBack(*(m_pHead + i));
				}
			}

			return arr;
		}
		constexpr Array FindAll(const UnaryPred& pred) const
		{
			Array arr{};

			const int32 size{ Size() };
			for (int32 i{}; i < size; ++i)
			{
				if (pred(*(m_pHead + i)))
				{
					arr.EmplaceBack(*(m_pHead + i));
				}
			}

			return arr;
		}
	#pragma endregion

	#pragma region Iterators
		constexpr It begin() { return m_pHead; }
		constexpr CIt begin() const { return m_pHead; }

		constexpr It end() { return m_pCurrentEnd; }
		constexpr CIt end() const { return m_pCurrentEnd; }

		constexpr CIt cbegin() const { return m_pHead; }
		constexpr CIt cend() const { return m_pCurrentEnd; }
	#pragma endregion

	private:
	#pragma region Internal Helpers
		template<bool AllowMove>
		constexpr void CopyData(T* const newHead, T* const oldHead, const int32 size) const
		{
			if constexpr (std::is_trivially_copyable_v<T>)
			{
				memcpy(newHead, oldHead, size * sizeof(T));
			}
			else if constexpr (AllowMove && std::is_move_constructible_v<T>)
			{
				for (int32 i{}; i < size; ++i)
				{
					new (newHead + i) T{ I_MOVE(*(oldHead + i)) };
				}
			}
			else
			{
				for (int32 i{}; i < size; ++i)
				{
					new (newHead + i) T{ *(oldHead + i) };
				}
			}
		}

		constexpr void Reallocate()
		{
			const int32 size{ Size() };
			const int32 newCap{ CalculateNewCapacity(size + 1) };

			T* const pNewHead{ static_cast<T*>(malloc(newCap * sizeof(T))) };

			CopyData<true>(pNewHead, m_pHead, size);

			DeleteData(m_pHead, m_pTail);
			Release(m_pHead);

			m_pHead = pNewHead;
			m_pTail = pNewHead + newCap;
			m_pCurrentEnd = pNewHead + size;
		}
		constexpr void ReallocateExactly(const int32 newCap)
		{
			const int32 size{ Size() };

			T* const pNewHead{ static_cast<T*>(malloc(newCap * sizeof(T))) };

			CopyData<true>(pNewHead, m_pHead, size);

			DeleteData(m_pHead, m_pTail);
			Release(m_pHead);

			m_pHead = pNewHead;
			m_pTail = pNewHead + newCap;
			m_pCurrentEnd = pNewHead + size;
		}

		constexpr void Release(T*& pData)
		{
			if (pData)
			{
				free(pData);
				pData = nullptr;
			}
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

		__NODISCARD constexpr int32 CalculateNewCapacity(const int32 min) const
		{
			const int32 oldCap{ Capacity() };
			const int32 maxCap{ MaxSize() };

			if (oldCap > maxCap - oldCap / 2)
			{
				return maxCap;
			}

			const int32 newCap{ oldCap + oldCap / 2 };

			// If our growth is insufficient, return just the bare minimum
			if (newCap < min)
			{
				return min;
			}

			return newCap;
		}

		constexpr void MoveRange(T* head, T* end, T* newHead) const
		{
			CopyData<true>(newHead, head, static_cast<int32>(end - head));
		}
	#pragma endregion

	#pragma region Sorters
		constexpr void InsertionSort(const BinaryPred& pred) const
		{
			/* Don't use At() to make sure elements get copied instead of referenced around! */

			const int32 size{ Size() };
			for (int i{ 1 }; i < size; ++i)
			{
				T key = *(m_pHead + i);
				int j{ i - 1 };

				while (j >= 0 && pred(key, *(m_pHead + j)))
				{
					*(m_pHead + j + 1) = *(m_pHead + j);
					--j;
				}

				*(m_pHead + j + 1) = key;
			}
		}
		constexpr void MergeSort(const int32 begin, const int32 end, const BinaryPred& pred) const
		{
			/* Check recursion stop */
			if (begin >= end)
			{
				return;
			}

			const int32 mid{ begin + (end - begin) / 2 };

			MergeSort(begin, mid, pred);
			MergeSort(mid + 1u, end, pred);
			Merge(begin, mid, end, pred);
		}
		constexpr void Merge(int32 left, int32 mid, const int32 right, const BinaryPred& pred) const
		{
			int32 halfStart{ mid + 1 };

			if (pred(*(m_pHead + mid), *(m_pHead + halfStart)))
			{
				return;
			}

			while (left <= mid && halfStart <= right)
			{
				if (pred(*(m_pHead + left), *(m_pHead + halfStart)))
				{
					++left;
				}
				else
				{
					T val{ *(m_pHead + halfStart) };
					int32 i{ halfStart };

					while (i != left)
					{
						*(m_pHead + i) = *(m_pHead + i - 1);
						--i;
					}

					*(m_pHead + left) = val;

					++left;
					++mid;
					++halfStart;
				}
			}
		}
	#pragma endregion

		T* m_pHead;
		T* m_pTail;
		T* m_pCurrentEnd /* points PAST the last element */;
	};

	template<typename T>
	using TArray = Array<T>;
}