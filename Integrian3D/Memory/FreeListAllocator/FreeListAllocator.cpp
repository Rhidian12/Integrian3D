#include "FreeListAllocator.h"

namespace Integrian3D::Memory
{
	FreeListAllocator::FreeListAllocator(const PlacementPolicy policy)
		: FreeListAllocator{ 32, policy }
	{}
	FreeListAllocator::FreeListAllocator(const uint64_t size, const PlacementPolicy policy)
		: m_pStart{}
		, m_PlacementPolicy{ policy }
		, m_FreeList{}
		, m_UsedList{}
		, m_Size{}
		, m_Capacity{ size }
	{
		// m_pStart = new char[size] {};
		m_pStart = malloc(m_Capacity);

		__ASSERT(m_pStart != nullptr && "Program ran out of memory");

		static_cast<FreeHeader*>(m_pStart)->Size = m_Capacity;
		Add(m_FreeList, static_cast<FreeHeader*>(m_pStart));

		// m_FreeList = static_cast<FreeHeader*>(malloc(sizeof(FreeHeader)));
		// m_FreeList->Size = m_Capacity;

		// m_FreeList.Add(static_cast<FreeHeader*>(m_pStart));
		// (*m_FreeList.Front())->Size = m_Capacity;
	}

	FreeListAllocator::~FreeListAllocator()
	{
		// delete[] m_pStart;
		free(m_pStart);
		m_pStart = nullptr;

		free(m_FreeList);
		m_FreeList = nullptr;

		free(m_UsedList);
		m_UsedList = nullptr;
	}

#pragma region RuleOf5
	FreeListAllocator::FreeListAllocator(const FreeListAllocator& other) noexcept
		: m_pStart{}
		, m_PlacementPolicy{ other.m_PlacementPolicy }
		, m_FreeList{}
		, m_UsedList{}
		, m_Size{}
		, m_Capacity{ other.m_Capacity }
	{
		// m_pStart = new char[other.m_Capacity]{};
		m_pStart = malloc(other.m_Capacity);

		__ASSERT(m_pStart != nullptr && "Program ran out of memory");

		static_cast<FreeHeader*>(m_pStart)->Size = m_Capacity;
		Add(m_FreeList, static_cast<FreeHeader*>(m_pStart));

		// m_FreeList.Add(static_cast<FreeHeader*>(m_pStart));
		// (*m_FreeList.Front())->Size = m_Capacity;
	}

	FreeListAllocator::FreeListAllocator(FreeListAllocator&& other) noexcept
		: m_pStart{ __MOVE(other.m_pStart) }
		, m_PlacementPolicy{ __MOVE(other.m_PlacementPolicy) }
		, m_FreeList{ __MOVE(other.m_FreeList) }
		, m_UsedList{ __MOVE(other.m_UsedList) }
		, m_Size{ __MOVE(other.m_Size) }
		, m_Capacity{ __MOVE(other.m_Capacity) }
	{
		other.m_pStart = nullptr;
		other.m_FreeList = nullptr;
		other.m_UsedList = nullptr;

		// other.m_FreeList.Clear();
		// other.m_UsedList.Clear();
	}

	FreeListAllocator& FreeListAllocator::operator=(const FreeListAllocator& other) noexcept
	{
		if (m_pStart)
		{
			// delete[] m_pStart;
			free(m_pStart);
			m_pStart = nullptr;
		}

		m_PlacementPolicy = other.m_PlacementPolicy;
		m_Capacity = other.m_Capacity;

		//m_pStart = new char[other.m_Capacity]{};
		m_pStart = malloc(other.m_Capacity);

		__ASSERT(m_pStart != nullptr && "Program ran out of memory");

		static_cast<FreeHeader*>(m_pStart)->Size = m_Capacity;
		Add(m_FreeList, static_cast<FreeHeader*>(m_pStart));

		// m_FreeList.Add(static_cast<FreeHeader*>(m_pStart));
		// (*m_FreeList.Front())->Size = m_Capacity;

		return *this;
	}

	FreeListAllocator& FreeListAllocator::operator=(FreeListAllocator&& other) noexcept
	{
		if (m_pStart)
		{
			// delete[] m_pStart;
			free(m_pStart);
			m_pStart = nullptr;
		}

		m_pStart = __MOVE(other.m_pStart);
		m_PlacementPolicy = __MOVE(other.m_PlacementPolicy);
		m_FreeList = __MOVE(other.m_FreeList);
		m_UsedList = __MOVE(other.m_UsedList);
		m_Size = __MOVE(other.m_Size);
		m_Capacity = __MOVE(other.m_Capacity);

		other.m_pStart = nullptr;
		other.m_FreeList = nullptr;
		other.m_UsedList = nullptr;

		// other.m_FreeList.Clear();
		// other.m_UsedList.Clear();

		return *this;
	}
#pragma endregion

	void* FreeListAllocator::Allocate(const uint64_t n, const uint64_t alignment)
	{
		__ASSERT(n > 0 && "Allocation size must be bigger");

		// Search through the free list for a free block that has enough space to allocate our data
		// padding takes alignment into account
		uint64_t padding{};
		FreeHeader* pFreeHeader{};
		Find(n, alignment, padding, pFreeHeader);

		const uint64_t totalAllocationSize{ n + padding };

		if (!pFreeHeader)
		{
			Reallocate(totalAllocationSize + sizeof(FreeHeader*));

			return Allocate(n, alignment);
		}

		const uint64_t rest{ pFreeHeader->Size - totalAllocationSize };

		if (rest > 0)
		{
			// We have to split the block into the data block and a free block of size 'rest'
			FreeHeader* pNewNode{ reinterpret_cast<FreeHeader*>(reinterpret_cast<uint64_t>(pFreeHeader) + totalAllocationSize) };
			pNewNode->Size = rest;

			Add(m_FreeList, pNewNode);
			// m_FreeList.Add(pNewNode);
		}

		Remove(m_FreeList, pFreeHeader);
		// m_FreeList.Erase(pFreeHeader);

		/* Set up data */
		AllocationHeader* pAllHeader{ reinterpret_cast<AllocationHeader*>(reinterpret_cast<uint64_t>(pFreeHeader) + sizeof(FreeHeader)) };
		pAllHeader->Size = n;
		pAllHeader->Padding = padding;

		Add(m_UsedList, pAllHeader);
		// m_UsedList.Add(pAllHeader);

		++m_Size;

		return reinterpret_cast<void*>(reinterpret_cast<uint64_t>(pAllHeader) + sizeof(AllocationHeader));
	}

	void FreeListAllocator::Deallocate(void* p)
	{
		if (!p)
			return;

		AllocationHeader* pAllHeader{ reinterpret_cast<AllocationHeader*>(reinterpret_cast<uint64_t>(p) - sizeof(AllocationHeader)) };
		FreeHeader* pFreeHeader{ reinterpret_cast<FreeHeader*>(reinterpret_cast<uint64_t>(pAllHeader) - sizeof(FreeHeader)) };
		pFreeHeader->Size = pAllHeader->Size + pAllHeader->Padding;

		Remove(m_UsedList, pAllHeader);
		// m_UsedList.Erase(pAllHeader);

		--m_Size;

		FreeHeader* pLastBlock{};

		const uint64_t nrOfFreeHeaders{ Count(m_FreeList) };
		// const uint64_t nrOfFreeHeaders{ m_FreeList.Size() };
		if (nrOfFreeHeaders > 0u)
		{
			pLastBlock = Get(m_FreeList, nrOfFreeHeaders - 1)->Data;
			// pLastBlock = *m_FreeList.Back();
		}

		if (nrOfFreeHeaders == 0u)
		{
			Add(m_FreeList, pFreeHeader);
			// m_FreeList.Add(pFreeHeader);
		}
		else if (reinterpret_cast<uint64_t>(pLastBlock) + pLastBlock->Size == reinterpret_cast<uint64_t>(pFreeHeader))
		{
			pLastBlock->Size += pFreeHeader->Size;
		}
		else
		{
			Add(m_FreeList, pFreeHeader);
			// m_FreeList.Add(pFreeHeader);
		}

		if (pLastBlock)
		{
			FreeHeader* pNewLastBlock{ Get(m_FreeList, Count(m_FreeList) - 1)->Data };
			// FreeHeader* pNewLastBlock{ *m_FreeList.Back() };

			if (reinterpret_cast<uint64_t>(pLastBlock) + pLastBlock->Size == reinterpret_cast<uint64_t>(pNewLastBlock))
			{
				pLastBlock->Size += pNewLastBlock->Size;
				Remove(m_FreeList, pNewLastBlock);
				// m_FreeList.Erase(pNewLastBlock);
			}
		}
	}

	void* FreeListAllocator::Get(const uint64_t n) const
	{
		if (n >= m_Size)
			return nullptr;

		return reinterpret_cast<void*>(reinterpret_cast<uint64_t>(Get(m_UsedList, n)->Data) + sizeof(AllocationHeader));
		// return reinterpret_cast<void*>(reinterpret_cast<uint64_t>(*m_UsedList.At(n)) + sizeof(AllocationHeader));
	}

	uint64_t FreeListAllocator::GetAllocationSize(const uint64_t n) const
	{
		if (n >= m_Size)
			return 0u;

		return Get(m_UsedList, n)->Data->Size;
		// return (*m_UsedList.At(n))->Size;
	}

	void FreeListAllocator::FindFirst(const uint64_t size, const uint64_t alignment, uint64_t& padding, FreeHeader*& pFreeHeader)
	{
		for (uint64_t i{}; i < Count(m_FreeList); ++i)
		{
			FreeHeader* pHeader{ Get(m_FreeList, i)->Data };
			// FreeHeader* pHeader{ *m_FreeList.At(i) };

			if (!pHeader)
				return;

			padding = AlignForward<FreeHeader, AllocationHeader>(&pHeader, alignment);

			const uint64_t requiredSpace{ size + padding };

			if (pHeader->Size >= requiredSpace + sizeof(FreeHeader*))
			{
				pFreeHeader = pHeader;
				break;
			}
		}
	}

	void FreeListAllocator::FindBest(const uint64_t size, const uint64_t alignment, uint64_t& padding, FreeHeader*& pFreeHeader)
	{
		FreeHeader* pBestFit{};
		for (uint64_t i{}; i < Count(m_FreeList); ++i)
		{
			FreeHeader* pHeader{ Get(m_FreeList, i)->Data };
			// FreeHeader* pHeader{ *m_FreeList.At(i) };

			if (!pHeader)
				return;

			padding = AlignForward<FreeHeader, AllocationHeader>(&pHeader, alignment);

			const uint64_t requiredSpace{ size + padding };

			if (pHeader->Size >= requiredSpace + sizeof(FreeHeader*))
			{
				if (pHeader->Size == requiredSpace + sizeof(FreeHeader*))
				{
					pFreeHeader = pHeader;
					break;
				}

				if (pBestFit)
				{
					if (pHeader->Size < pBestFit->Size)
						pBestFit = pHeader;
				}
				else
					pBestFit = pHeader;
			}
		}

		pFreeHeader = pBestFit;
	}

	void FreeListAllocator::Reallocate(const uint64_t requiredSize)
	{
		const uint64_t newCap{ CalculateNewCapacity(m_Capacity + requiredSize + m_NodePadding + 1u) };
		FreeHeader* const pNewStart{ static_cast<FreeHeader*>(malloc(newCap)) };
		// FreeHeader* const pNewStart{ reinterpret_cast<FreeHeader*>(new char[newCap] {}) };

		__ASSERT(pNewStart != nullptr && "Program ran out of memory");

		pNewStart->Size = newCap;

		Node<FreeHeader*>* freeList{};
		Node<AllocationHeader*>* usedList{};
		Add(freeList, pNewStart);
		// LinkedList<FreeHeader*> freeList{};
		// LinkedList<AllocationHeader*> usedList{};
		// freeList.Add(pNewStart);
		uint64_t totalAllocatedMem{};

		if (Count(m_UsedList) > 0u)
		{
			/* loop over every allocated piece of memory and copy it over */
			for (uint64_t i{}; i < Count(m_UsedList); ++i)
			{
				/* get currently allocated memory */
				AllocationHeader* pOldAllHeader{ Get(m_UsedList, i)->Data };
				// AllocationHeader* pOldAllHeader{ *m_UsedList.At(i) };

				/* get our new free header */
				FreeHeader* pFreeHeader{ reinterpret_cast<FreeHeader*>(reinterpret_cast<uint64_t>(pNewStart) + totalAllocatedMem) };

				/* initialize new allocation header */
				AllocationHeader* pNewAllHeader{ reinterpret_cast<AllocationHeader*>(reinterpret_cast<uint64_t>(pFreeHeader) + sizeof(FreeHeader)) };
				pNewAllHeader->Size = pOldAllHeader->Size;
				pNewAllHeader->Padding = pOldAllHeader->Padding;

				/* copy over data */
				memcpy
				(
					reinterpret_cast<void*>(reinterpret_cast<uint64_t>(pNewAllHeader) + sizeof(AllocationHeader)),
					reinterpret_cast<void*>(reinterpret_cast<uint64_t>(pOldAllHeader) + sizeof(AllocationHeader)),
					pOldAllHeader->Size
				);

				/* make new free header from current free header */
				const uint64_t totalSize{ pOldAllHeader->Size + pOldAllHeader->Padding };
				FreeHeader* pNewFreeHeader{ reinterpret_cast<FreeHeader*>(reinterpret_cast<uint64_t>(pFreeHeader) + totalSize) };
				pNewFreeHeader->Size = totalSize;

				/* keep track of how much memory we've currently copied over */
				totalAllocatedMem += totalSize;

				Remove(freeList, pFreeHeader);
				Add(freeList, pNewFreeHeader);
				Add(usedList, pNewAllHeader);
				// freeList.Erase(pFreeHeader);
				// freeList.Add(pNewFreeHeader);
				// usedList.Add(pNewAllHeader);
			}

			/* copium let's assume we have an element remaining, we should tho */
			/* I'm adding too much memory, because this can't be equal to newCap, especially if memory has been allocated */

			Get(freeList, Count(freeList) - 1)->Data->Size = newCap - totalAllocatedMem;
			// (*freeList.Back())->Size = newCap - totalAllocatedMem;
		}

		__ASSERT(Count(usedList) == Count(m_UsedList));

		m_FreeList = __MOVE(freeList);
		m_UsedList = __MOVE(usedList);

		free(m_pStart);
		// delete[] m_pStart;

		m_pStart = pNewStart;
		m_Capacity = newCap;
	}
}
