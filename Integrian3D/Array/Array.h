#pragma once

#include "../EngineConstants.h"
#include "../Iterator/Iterator.h"
#include "../Memory/Allocator/Allocator.h"
#include "../Memory/FreeListAllocator/FreeListAllocator.h"
#include "../Types/Types.h"

#include <functional> /* std::function */

namespace Integrian3D
{
	template<typename T, typename Alloc>
	class Array
	{
		using UnaryPred = std::function<bool(const T&)>;
		using Handle = Memory::Allocator<Alloc>::template Handle<T>;

	public:
		using It = Iterator<T, IteratorTag::RandomAccessIt>;
		using CIt = ConstIterator<T, IteratorTag::RandomAccessIt>;

#pragma region Ctors and Dtor
		constexpr Array(const Alloc& alloc = Alloc{})
			: m_Allocator{ Memory::Allocator<Alloc>{ alloc } }
			, m_pHandle{}
			, m_Size{}
		{}
		constexpr Array(const Size_P size, const Alloc& alloc = Alloc{})
			: m_Allocator{ Memory::Allocator<Alloc>{ alloc } }
			, m_pHandle{}
			, m_Size{}
		{
			Reserve(size._Size);

			for (uint64_t i{}; i < size._Size; ++i)
			{
				EmplaceBack(T{});
			}
		}
		constexpr Array(const Size_P size, const T& val, const Alloc& alloc = Alloc{})
			: m_Allocator{ Memory::Allocator<Alloc>{ alloc } }
			, m_pHandle{}
			, m_Size{}
		{
			Reserve(size._Size);

			for (uint64_t i{}; i < size._Size; ++i)
			{
				EmplaceBack(val);
			}
		}
		constexpr Array(const Capacity_P cap, const Alloc& alloc = Alloc{})
			: m_Allocator{ Memory::Allocator<Alloc>{ alloc } }
			, m_pHandle{}
			, m_Size{}
		{
			Reserve(cap._Capacity);
		}
		constexpr Array(std::initializer_list<T> init, const Alloc& alloc = Alloc{})
			: m_Allocator{ Memory::Allocator<Alloc>{ alloc } }
			, m_pHandle{}
			, m_Size{}
		{
			Reserve(init.size());

			for (const T& elem : init)
			{
				EmplaceBack(elem);
			}
		}
		constexpr Array(It beg, It end, const Alloc& alloc = Alloc{})
			: m_Allocator{ Memory::Allocator<Alloc>{ alloc } }
			, m_pHandle{}
			, m_Size{}
		{
			Reserve(*end - *beg);

			for (; beg != end; ++beg)
			{
				EmplaceBack(*beg);
			}
		}

		constexpr ~Array()
		{
			if (m_pHandle)
			{
				m_Allocator.Deallocate(*m_pHandle);
				m_pHandle = nullptr;
			}
		}
#pragma endregion

#pragma region RuleOf5
		constexpr Array(const Array& other) noexcept
			: m_Allocator{ other.m_Allocator }
			, m_pHandle{}
			, m_Size{ other.m_Size }
		{
			const uint64_t cap{ other.Capacity() };
			m_pHandle = std::addressof(m_Allocator.Allocate<T>(cap));

			const uint64_t size{ other.Size() };
			for (uint64_t i{}; i < size; ++i)
			{
				new (&(*m_pHandle) + i) T{ *(&(*other.m_pHandle) + i) }; // dont allow moving 
			}
		}
		constexpr Array(Array&& other) noexcept
			: m_Allocator{ __MOVE(other.m_Allocator) }
			, m_pHandle{ static_cast<Handle*>(m_Allocator.GetHandle(other.m_pHandle->GetMemoryLocation())) }
			, m_Size{ __MOVE(other.m_Size) }
		{
			other.m_pHandle = nullptr;
			other.m_Size = 0;
		}

		constexpr Array& operator=(const Array& other) noexcept
		{
			if (m_Allocator.Size() > 0)
			{
				if (m_pHandle)
				{
					m_Allocator.Deallocate(*m_pHandle);
					m_pHandle = nullptr;
				}
			}

			m_Allocator = other.m_Allocator;

			const uint64_t cap{ other.Capacity() };
			m_pHandle = std::addressof(m_Allocator.Allocate<T>(cap));

			const uint64_t size{ other.Size() };
			for (uint64_t i{}; i < size; ++i)
			{
				new (&(*m_pHandle) + i) T{ *(&(*other.m_pHandle) + i) }; // dont allow moving 
			}

			return *this;
		}
		constexpr Array& operator=(Array&& other) noexcept
		{
			if (m_Allocator.Size() > 0)
			{
				if (m_pHandle)
				{
					m_Allocator.Deallocate(*m_pHandle);
					m_pHandle = nullptr;
				}
			}

			m_Allocator = __MOVE(other.m_Allocator);
			m_pHandle = static_cast<Handle*>(m_Allocator.GetHandle(other.m_pHandle->GetMemoryLocation()));
			m_Size = __MOVE(other.m_Size);

			other.m_pHandle = nullptr;
			other.m_Size = 0;

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
			EmplaceBack(__MOVE(val));
		}

		constexpr void AddFront(const T& val)
		{
			EmplaceFront(val);
		}
		constexpr void AddFront(T&& val)
		{
			EmplaceFront(__MOVE(val));
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
			for (; beg != end; ++beg)
			{
				EmplaceBack(*beg);
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

				return begin();
			}
			else
			{
				(&(*m_pHandle) + index)->~T();
				MoveRange(&(*m_pHandle) + index + 1, &(*m_pHandle) + m_Size--, &(*m_pHandle) + index);

				return It{ &(*m_pHandle) + index };
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
				if (*(&(*m_pHandle) + i) == val)
				{
					return Erase(i);
					break; /* [TODO]: Looks cringe */
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
			It it{ Find(__MOVE(pred)) };

			if (it != end())
			{
				return Erase(*it);
			}

			return end();
		}

		constexpr void EraseRange(const uint64_t start, const uint64_t count)
		{
			__ASSERT(start < Size() && "Array::EraseRange() > Start is out of range");

			EraseRange(It{ &(*m_pHandle) + start }, It{ &(*m_pHandle) + start + count });
		}
		constexpr void EraseRange(It beg, It endIt)
		{
			__ASSERT(beg != end() && "Array::EraseRange() > Cannot iterator past the end");

			if (endIt >= end())
			{
				endIt = It{ &(*m_pHandle) + m_Size - 1 };
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
			Emplace(index, __MOVE(val));
		}

		constexpr void Pop()
		{
			if (Size() == 0)
			{
				return;
			}

			m_Allocator.Destroy(&(*m_pHandle) + --m_Size);
			// (&(*m_Handle) + --m_Size)->~T();
		}

		constexpr void PopFront()
		{
			if (Size() == 0)
			{
				return;
			}

			m_Allocator.Destroy(&(*m_pHandle));
			//(&(*m_Handle))->~T();

			MoveRange(&(*m_pHandle) + 1, &(*m_pHandle) + m_Size--, &(*m_pHandle));
		}

		constexpr void Clear()
		{
			//if (m_pHandle)
			//{
			//	m_Allocator.Deallocate(*m_pHandle);
			//	m_pHandle = nullptr;
			//}

			for (uint64_t i{}; i < m_Size; ++i)
				m_Allocator.Destroy(&(*m_pHandle) + i);

			m_Size = 0;
		}

		template<typename ... Ts>
		constexpr T& EmplaceBack(Ts&&... args)
		{
			if (m_Size >= Capacity())
			{
				Reallocate();
			}

			return *(new (&(*m_pHandle) + m_Size++) T{ __FORWARD(args)... });
		}

		template<typename ... Ts>
		constexpr T& Emplace(const uint64_t index, Ts&&... args)
		{
			__ASSERT(index < Size() && "Array::Emplace() > index is out of range");

			if (m_Size + 1 >= Capacity())
			{
				Reallocate();
			}

			if (m_Size == 0 || index == m_Size - 1)
			{
				return EmplaceBack(__FORWARD(args)...);
			}
			else
			{
				MoveRange(&(*m_pHandle) + index, &(*m_pHandle) + m_Size++ - 1, &(*m_pHandle) + index + 1);
				return *(new (&(*m_pHandle) + index) T{ __FORWARD(args)... });
			}
		}

		template<typename ... Ts>
		constexpr T& EmplaceFront(Ts&&... args)
		{
			if (m_Size >= Capacity())
			{
				Reallocate();
			}

			MoveRange(&(*m_pHandle), &(*m_pHandle) + m_Size++, &(*m_pHandle) + 1);

			return *(new (&(*m_pHandle)) T{ __FORWARD(args)... });
		}
#pragma endregion

#pragma region Array Information
		__NODISCARD constexpr bool Empty() const
		{
			return m_Size == 0;
		}

		__NODISCARD constexpr uint64_t Size() const
		{
			return m_Size;
		}

		__NODISCARD constexpr uint64_t Capacity() const
		{
			if (!m_pHandle) /* [CRINGE]: AN IF STATEMENT FOR EACH CAPACITY CALL?? */
				return 0u;

			return m_pHandle->Size() / sizeof(T);
		}

		__NODISCARD constexpr uint64_t MaxSize() const
		{
			return std::numeric_limits<uint64_t>::max();
		}

		__NODISCARD constexpr bool operator==(const Array& other) const
		{
			if (m_Size != other.m_Size)
			{
				return false;
			}

			for (uint64_t i{}; i < m_Size; ++i)
			{
				if (*(&(*m_pHandle) + i) != *(&(*other.m_pHandle) + i))
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

			if (newSize > m_Size)
			{
				const uint64_t diff{ newSize - m_Size };

				for (uint64_t i{}; i < diff; ++i)
				{
					EmplaceBack(__FORWARD(val));
				}

				return;
			}

			if (newSize < m_Size)
			{
				const uint64_t diff{ m_Size - newSize };

				for (uint64_t i{}; i < diff; ++i)
				{
					Pop();
				}

				return;
			}
		}

		constexpr void ShrinkToFit()
		{
			if (m_Size == Capacity())
				return;

			ReallocateExactly(m_Size);
		}

		constexpr Array Select(const UnaryPred& pred) const
		{
			Array arr{ Capacity_P{ m_Size }, m_Allocator.GetAllocator() };

			for (uint64_t i{}; i < m_Size; ++i)
			{
				const T* const elem{ &(*m_pHandle) + i };

				if (pred(*elem))
				{
					arr.Add(*elem);
				}
			}

			return arr;
		}
		constexpr Array Select(UnaryPred&& pred) const
		{
			Array arr{ Capacity_P{ m_Size }, m_Allocator.GetAllocator() };

			for (uint64_t i{}; i < m_Size; ++i)
			{
				const T* const elem{ &(*m_pHandle) + i };

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
			__ASSERT(m_Size > 0 && "Array::Front() > Array is empty");

			return *(&(*m_pHandle));
		}
		constexpr const T& Front() const
		{
			__ASSERT(m_Size > 0 && "Array::Front() > Array is empty");

			return *(&(*m_pHandle));
		}

		constexpr T& Back()
		{
			__ASSERT(m_Size > 0 && "Array::Back() > Array is empty");

			return *(&(*m_pHandle) + m_Size - 1);
		}
		constexpr const T& Back() const
		{
			__ASSERT(m_Size > 0 && "Array::Back() > Array is empty");

			return *(&(*m_pHandle) + m_Size - 1);
		}

		constexpr T& At(const uint64_t index)
		{
			__ASSERT((index < m_Size) && "Array::At() > Index is out of range");

			return *(&(*m_pHandle) + index);
		}
		constexpr const T& At(const uint64_t index) const
		{
			__ASSERT((index < m_Size) && "Array::At() > Index is out of range");

			return *(&(*m_pHandle) + index);
		}

		constexpr T& operator[](const uint64_t index)
		{
			return *(&(*m_pHandle) + index);
		}
		constexpr const T& operator[](const uint64_t index) const
		{
			return *(&(*m_pHandle) + index);
		}

		constexpr T* const Data()
		{
			if (!m_pHandle)
				return nullptr;

			return &(*m_pHandle);
		}
		constexpr const T* const Data() const
		{
			if (!m_pHandle)
				return nullptr;

			return &(*m_pHandle);
		}

		constexpr It Find(const T& val)
		{
			for (uint64_t i{}; i < m_Size; ++i)
			{
				if (*(&(*m_pHandle) + i) == val)
				{
					return It{ &(*m_pHandle) + i };
				}
			}

			return It{ &(*m_pHandle) + m_Size };
		}
		constexpr CIt Find(const T& val) const
		{
			for (uint64_t i{}; i < m_Size; ++i)
			{
				if (*(&(*m_pHandle) + i) == val)
				{
					return CIt{ &(*m_pHandle) + i };
				}
			}

			return CIt{ &(*m_pHandle) + m_Size };
		}
		constexpr It Find(const UnaryPred& pred)
		{
			for (uint64_t i{}; i < m_Size; ++i)
			{
				if (pred(*(&(*m_pHandle) + i)))
				{
					return It{ &(*m_pHandle) + i };
				}
			}

			return It{ &(*m_pHandle) + m_Size };
		}
		constexpr CIt Find(const UnaryPred& pred) const
		{
			for (uint64_t i{}; i < m_Size; ++i)
			{
				if (pred(*(&(*m_pHandle) + i)))
				{
					return CIt{ &(*m_pHandle) + i };
				}
			}

			return CIt{ &(*m_pHandle) + m_Size };
		}

		constexpr Array FindAll(const T& val) const
		{
			Array arr{};

			for (uint64_t i{}; i < m_Size; ++i)
			{
				if (*(&(*m_pHandle) + i) == val)
				{
					arr.EmplaceBack(*(&(*m_pHandle) + i));
				}
			}

			return arr;
		}
		constexpr Array FindAll(const UnaryPred& pred) const
		{
			Array arr{};

			for (uint64_t i{}; i < m_Size; ++i)
			{
				if (pred(*(&(*m_pHandle) + i)))
				{
					arr.EmplaceBack(*(&(*m_pHandle) + i));
				}
			}

			return arr;
		}
		constexpr Array FindAll(UnaryPred&& pred) const
		{
			Array arr{};

			for (uint64_t i{}; i < m_Size; ++i)
			{
				if (pred(*(&(*m_pHandle) + i)))
				{
					arr.EmplaceBack(*(&(*m_pHandle) + i));
				}
			}

			return arr;
		}
#pragma endregion

#pragma region Iterators
		constexpr It begin() { return &(*m_pHandle); }
		constexpr CIt begin() const { return &(*m_pHandle); }

		constexpr It end() { return &(*m_pHandle) + m_Size; }
		constexpr CIt end() const { return &(*m_pHandle) + m_Size; }

		constexpr CIt cbegin() const { return &(*m_pHandle); }
		constexpr CIt cend() const { return &(*m_pHandle) + m_Size; }
#pragma endregion

	private:
#pragma region Internal Helpers
		constexpr void Reallocate()
		{
			auto newHandle{ std::addressof(m_Allocator.Allocate<T>(CalculateNewCapacity(m_Size + 1))) };

			if (m_pHandle)
			{
				m_Allocator.Deallocate(*m_pHandle);
				m_pHandle = nullptr;
			}

			m_pHandle = newHandle;
		}
		constexpr void ReallocateExactly(const uint64_t newCap)
		{
			auto newHandle{ std::addressof(m_Allocator.Allocate<T>(newCap)) };

			if (m_pHandle)
			{
				m_Allocator.Deallocate(*m_pHandle);
				m_pHandle = nullptr;
			}

			m_pHandle = newHandle;
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

			const uint64_t size{ static_cast<uint64_t>(end - head) };
			for (uint64_t i{}; i < size; ++i)
			{
				if constexpr (std::is_move_assignable_v<T>)
				{
					new (newHead + i) T{ __MOVE(*(head + i)) };
				}
				else
				{
					new (newHead + i) T{ *(head + i) };
				}
			}
		}
#pragma endregion

		Memory::Allocator<Alloc> m_Allocator;
		Handle* m_pHandle;
		uint64_t m_Size;
	};

	template<typename T>
	using TArray = Array<T, Memory::FreeListAllocator>;
}