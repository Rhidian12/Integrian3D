#include "FreeListAllocator.h"

namespace Integrian3D::Memory
{
#pragma region Page

	FreeListAllocator::Page::Page(const uint64_t size)
		: m_pStart{}
		, m_FreeHeaders{}
		, m_UsedHeaders{}
		, m_PageSize{ size }
	{
		m_pStart = malloc(size);

		__CHECK(m_pStart != nullptr && "Ran out of memory");

		static_cast<FreeHeader*>(m_pStart)->Size = size;
		m_FreeHeaders.Add(static_cast<FreeHeader*>(m_pStart));
	}

	FreeListAllocator::Page::~Page()
	{
		m_FreeHeaders.Clear();
		m_UsedHeaders.Clear();
		free(m_pStart);
		m_pStart = nullptr;
	}

	FreeListAllocator::Page::Page(const Page& other) noexcept
		: m_pStart{}
		, m_FreeHeaders{}
		, m_UsedHeaders{}
		, m_PageSize{ other.GetPageSize() }
	{
		m_pStart = malloc(m_PageSize);

		__CHECK(m_pStart != nullptr && "Ran out of memory");

		static_cast<FreeHeader*>(m_pStart)->Size = m_PageSize;
		m_FreeHeaders.Add(static_cast<FreeHeader*>(m_pStart));
	}

	FreeListAllocator::Page::Page(Page&& other) noexcept
		: m_pStart{ __MOVE(other.m_pStart) }
		, m_FreeHeaders{ __MOVE(other.m_FreeHeaders) }
		, m_UsedHeaders{ __MOVE(other.m_UsedHeaders) }
		, m_PageSize{ __MOVE(other.m_PageSize) }
	{
		other.m_pStart = nullptr;
		other.m_FreeHeaders.Clear();
		other.m_UsedHeaders.Clear();
	}

	FreeListAllocator::Page& FreeListAllocator::Page::operator=(const Page& other) noexcept
	{
		if (m_pStart)
		{
			free(m_pStart);
			m_FreeHeaders.Clear();
			m_UsedHeaders.Clear();
		}

		m_pStart = malloc(other.m_PageSize);
		m_PageSize = other.m_PageSize;

		__CHECK(m_pStart != nullptr && "Ran out of memory");

		static_cast<FreeHeader*>(m_pStart)->Size = other.m_PageSize;
		m_FreeHeaders.Add(static_cast<FreeHeader*>(m_pStart));

		return *this;
	}

	FreeListAllocator::Page& FreeListAllocator::Page::operator=(Page&& other) noexcept
	{
		if (m_pStart)
		{
			free(m_pStart);
			m_FreeHeaders.Clear();
			m_UsedHeaders.Clear();
		}

		m_pStart = __MOVE(other.m_pStart);
		m_FreeHeaders = __MOVE(other.m_FreeHeaders);
		m_UsedHeaders = __MOVE(other.m_UsedHeaders);
		m_PageSize = __MOVE(other.m_PageSize);

		other.m_pStart = nullptr;
		other.m_FreeHeaders.Clear();
		other.m_UsedHeaders.Clear();

		return *this;
	}

	void FreeListAllocator::Page::AddFreeHeader(FreeHeader* pFreeHeader)
	{
		m_FreeHeaders.Add(pFreeHeader);
	}

	void FreeListAllocator::Page::RemoveFreeHeader(FreeHeader* pFreeHeader)
	{
		m_FreeHeaders.Remove(pFreeHeader);
	}

	bool FreeListAllocator::Page::HasFreeHeader(FreeHeader* pFreeHeader) const
	{
		return m_FreeHeaders.Contains(pFreeHeader);
	}

	FreeListAllocator::FreeHeader* FreeListAllocator::Page::GetFreeHeader(const uint64_t index)
	{
		return m_FreeHeaders.Get(index);
	}

	void FreeListAllocator::Page::AddUsedHeader(AllocationHeader* pAllHeader)
	{
		m_UsedHeaders.Add(pAllHeader);
	}

	void FreeListAllocator::Page::RemoveUsedHeader(AllocationHeader* pAllHeader)
	{
		m_UsedHeaders.Remove(pAllHeader);
	}

	bool FreeListAllocator::Page::HasAllocationHeader(AllocationHeader* pAllHeader) const
	{
		return m_UsedHeaders.Contains(pAllHeader);
	}

	void FreeListAllocator::Page::MergeFreeHeaders()
	{
		static int count{};

		for (uint64_t i{}; i < m_FreeHeaders.Count(); ++i)
		{
			/* Check if this free header and any other can be easily merged */
			/* i.e. are they adjacent? */
			FreeHeader* const pFreeHeader{ m_FreeHeaders.Get(i) };

			for (uint64_t j{}; j < m_FreeHeaders.Count(); ++j)
			{
				if (i == j)
					continue;

				FreeHeader* const pNextHeader{ m_FreeHeaders.Get(j) };

				if (reinterpret_cast<uint64_t>(pFreeHeader) + pFreeHeader->Size == reinterpret_cast<uint64_t>(pNextHeader))
				{
					pFreeHeader->Size += pNextHeader->Size;

					++count;

					m_FreeHeaders.Remove(pNextHeader);
				}
			}
		}
	}

	FreeListAllocator::FreeHeader* FreeListAllocator::Page::FindFirstFreeHeader(const uint64_t size, const uint64_t alignment, uint64_t& padding) const
	{
		for (uint64_t i{}; i < m_FreeHeaders.Count(); ++i)
		{
			FreeHeader* pHeader{ m_FreeHeaders.Get(i) };

			if (!pHeader)
				return nullptr;

			padding = AlignForward<FreeHeader, AllocationHeader>(&pHeader, alignment);

			const uint64_t requiredSpace{ size + padding };

			if (pHeader->Size >= requiredSpace + sizeof(FreeHeader*))
				return pHeader;
		}

		return nullptr;
	}

	FreeListAllocator::FreeHeader* FreeListAllocator::Page::FindBestFreeHeader(const uint64_t size, const uint64_t alignment, uint64_t& padding) const
	{
		FreeHeader* pBestFit{};
		for (uint64_t i{}; i < m_FreeHeaders.Count(); ++i)
		{
			FreeHeader* pHeader{ m_FreeHeaders.Get(i) };

			if (!pHeader)
				return nullptr;

			padding = AlignForward<FreeHeader, AllocationHeader>(&pHeader, alignment);

			const uint64_t requiredSpace{ size + padding };

			if (pHeader->Size >= requiredSpace + sizeof(FreeHeader*))
			{
				if (pHeader->Size == requiredSpace + sizeof(FreeHeader*))
					return pHeader;

				if (pBestFit)
				{
					if (pHeader->Size < pBestFit->Size)
						pBestFit = pHeader;
				}
				else
				{
					pBestFit = pHeader;
				}
			}
		}

		return pBestFit;
	}

	uint64_t FreeListAllocator::Page::GetPageSize() const
	{
		return m_PageSize;
	}

	uint64_t FreeListAllocator::Page::GetNrOfFreeHeaders() const
	{
		return m_FreeHeaders.Count();
	}

	uint64_t FreeListAllocator::Page::GetNrOfUsedHeaders() const
	{
		return m_UsedHeaders.Count();
	}

#pragma endregion

	FreeListAllocator::FreeListAllocator(const PlacementPolicy policy)
		: FreeListAllocator{ 4096, policy }
	{}
	FreeListAllocator::FreeListAllocator(const uint64_t size, const PlacementPolicy policy)
		: m_PlacementPolicy{ policy }
		, m_Pages{}
	{
		m_Pages.Add(__MOVE(Page{ size }));
	}

#pragma region RuleOf5
	FreeListAllocator::FreeListAllocator(const FreeListAllocator& other) noexcept
		: m_PlacementPolicy{ other.m_PlacementPolicy }
		, m_Pages{}
	{
		for (uint64_t i{}; i < other.m_Pages.Count(); ++i)
			m_Pages.Add(__MOVE(Page{ other.m_Pages.Get(i).GetPageSize() }));
	}

	FreeListAllocator::FreeListAllocator(FreeListAllocator&& other) noexcept
		: m_PlacementPolicy{ __MOVE(other.m_PlacementPolicy) }
		, m_Pages{ __MOVE(other.m_Pages) }
	{
		other.m_Pages.Clear();
	}

	FreeListAllocator& FreeListAllocator::operator=(const FreeListAllocator& other) noexcept
	{
		if (m_Pages.Count() > 0)
			m_Pages.Clear();

		m_PlacementPolicy = other.m_PlacementPolicy;
		m_Pages = other.m_Pages;

		for (uint64_t i{}; i < other.m_Pages.Count(); ++i)
			m_Pages.Add(__MOVE(Page{ other.m_Pages.Get(i).GetPageSize() }));

		return *this;
	}

	FreeListAllocator& FreeListAllocator::operator=(FreeListAllocator&& other) noexcept
	{
		if (m_Pages.Count() > 0)
			m_Pages.Clear();

		m_PlacementPolicy = __MOVE(other.m_PlacementPolicy);
		m_Pages = __MOVE(other.m_Pages);

		other.m_Pages.Clear();

		return *this;
	}
#pragma endregion

	void* FreeListAllocator::Allocate(const uint64_t n, const uint64_t alignment)
	{
		__CHECK(n > 0 && "Allocation size must be bigger");

		// Search through the free list for a free block that has enough space to allocate our data
		// padding takes alignment into account
		uint64_t padding{};
		Page* pPage{};
		FreeHeader* pFreeHeader{ Find(n, alignment, padding, pPage) };

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

			pPage->AddFreeHeader(pNewNode);
		}

		pPage->RemoveFreeHeader(pFreeHeader);

		/* Set up data */
		AllocationHeader* pAllHeader{ reinterpret_cast<AllocationHeader*>(reinterpret_cast<uint64_t>(pFreeHeader) + sizeof(FreeHeader)) };
		pAllHeader->Size = n;
		pAllHeader->Padding = padding;

		pPage->AddUsedHeader(pAllHeader);

		return reinterpret_cast<void*>(reinterpret_cast<uint64_t>(pAllHeader) + sizeof(AllocationHeader));
	}

	void FreeListAllocator::Deallocate(void* p)
	{
		if (!p)
			return;

		AllocationHeader* pAllHeader{ reinterpret_cast<AllocationHeader*>(reinterpret_cast<uint64_t>(p) - sizeof(AllocationHeader)) };
		FreeHeader* pFreeHeader{ reinterpret_cast<FreeHeader*>(reinterpret_cast<uint64_t>(pAllHeader) - sizeof(FreeHeader)) };
		pFreeHeader->Size = pAllHeader->Size + pAllHeader->Padding;

		Page* pPage{};
		for (uint64_t i{}; i < m_Pages.Count(); ++i)
		{
			if (m_Pages.Get(i).HasAllocationHeader(pAllHeader))
			{
				pPage = &m_Pages.Get(i);
				break;
			}
		}

		pPage->RemoveUsedHeader(pAllHeader);

		FreeHeader* pLastBlock{};

		const uint64_t nrOfFreeHeaders{ pPage->GetNrOfFreeHeaders() };
		if (nrOfFreeHeaders > 0u)
			pLastBlock = pPage->GetFreeHeader(nrOfFreeHeaders - 1);

		if (nrOfFreeHeaders == 0u)
			pPage->AddFreeHeader(pFreeHeader);
		else if (reinterpret_cast<uint64_t>(pLastBlock) + pLastBlock->Size == reinterpret_cast<uint64_t>(pFreeHeader))
			pLastBlock->Size += pFreeHeader->Size;
		else
			pPage->AddFreeHeader(pFreeHeader);

		if (pLastBlock)
		{
			FreeHeader* pNewLastBlock{ pPage->GetFreeHeader(pPage->GetNrOfFreeHeaders() - 1) };

			if (reinterpret_cast<uint64_t>(pLastBlock) + pLastBlock->Size == reinterpret_cast<uint64_t>(pNewLastBlock))
			{
				pLastBlock->Size += pNewLastBlock->Size;
				pPage->RemoveFreeHeader(pNewLastBlock);
			}
		}

		pPage->MergeFreeHeaders();
	}

	void* FreeListAllocator::Get(const uint64_t) const
	{
		//if (n >= m_Size)
		//	return nullptr;

		//return reinterpret_cast<void*>(reinterpret_cast<uint64_t>(Get(m_UsedList, n)->Data) + sizeof(AllocationHeader));
		//// return reinterpret_cast<void*>(reinterpret_cast<uint64_t>(*m_UsedList.At(n)) + sizeof(AllocationHeader));

		return nullptr;
	}

	uint64_t FreeListAllocator::GetAllocationSize(const uint64_t) const
	{
		//if (n >= m_Size)
		//	return 0u;

		//return Get(m_UsedList, n)->Data->Size;
		// return (*m_UsedList.At(n))->Size;

		return 0u;
	}

	uint64_t FreeListAllocator::Capacity() const
	{
		uint64_t cap{};

		for (uint64_t i{}; i < m_Pages.Count(); ++i)
			cap += m_Pages.Get(i).GetPageSize();

		return cap;
	}

	uint64_t FreeListAllocator::Size() const
	{
		uint64_t size{};

		for (uint64_t i{}; i < m_Pages.Count(); ++i)
			size += m_Pages.Get(i).GetNrOfUsedHeaders();

		return size;
	}

	FreeListAllocator::FreeHeader* FreeListAllocator::Find(const uint64_t size, const uint64_t alignment, uint64_t& padding, Page*& pPage)
	{
		switch (m_PlacementPolicy)
		{
		case PlacementPolicy::FIND_FIRST_FIT:
			return FindFirst(size, alignment, padding, pPage);
		case PlacementPolicy::FIND_BEST_FIT:
			return FindBest(size, alignment, padding, pPage);
		}

		return nullptr;
	}

	FreeListAllocator::FreeHeader* FreeListAllocator::FindFirst(const uint64_t size, const uint64_t alignment, uint64_t& padding, Page*& pPage)
	{
		for (uint64_t i{}; i < m_Pages.Count(); ++i)
		{
			FreeHeader* pHeader{ m_Pages.Get(i).FindFirstFreeHeader(size, alignment, padding) };

			if (pHeader)
			{
				pPage = &m_Pages.Get(i);
				return pHeader;
			}
		}

		return nullptr;
	}

	FreeListAllocator::FreeHeader* FreeListAllocator::FindBest(const uint64_t size, const uint64_t alignment, uint64_t& padding, Page*& pPage)
	{
		for (uint64_t i{}; i < m_Pages.Count(); ++i)
		{
			FreeHeader* pHeader{ m_Pages.Get(i).FindBestFreeHeader(size, alignment, padding) };

			if (pHeader)
			{
				pPage = &m_Pages.Get(i);
				return pHeader;
			}
		}

		return nullptr;
	}

	uint64_t FreeListAllocator::CalculateNewCapacity(const uint64_t min) const
	{
		const uint64_t oldCap{ Capacity() };
		const uint64_t maxCap{ std::numeric_limits<uint64_t>::max() };

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

	void FreeListAllocator::Reallocate(const uint64_t requiredSize)
	{
		const uint64_t newCap{ CalculateNewCapacity(m_Pages.Back().GetPageSize() + requiredSize + m_NodePadding + 1u) };
		m_Pages.Add(__MOVE(Page{ newCap }));

		//const uint64_t newCap{ CalculateNewCapacity(m_Capacity + requiredSize + m_NodePadding + 1u) };
		//FreeHeader* const pNewStart{ static_cast<FreeHeader*>(malloc(newCap)) };
		//// FreeHeader* const pNewStart{ reinterpret_cast<FreeHeader*>(new char[newCap] {}) };

		//__ASSERT(pNewStart != nullptr && "Program ran out of memory");

		//pNewStart->Size = newCap;

		//Node<FreeHeader*>* freeList{};
		//Node<AllocationHeader*>* usedList{};
		//Add(freeList, pNewStart);
		//// LinkedList<FreeHeader*> freeList{};
		//// LinkedList<AllocationHeader*> usedList{};
		//// freeList.Add(pNewStart);
		//uint64_t totalAllocatedMem{};

		//if (Count(m_UsedList) > 0u)
		//{
		//	/* loop over every allocated piece of memory and copy it over */
		//	for (uint64_t i{}; i < Count(m_UsedList); ++i)
		//	{
		//		/* get currently allocated memory */
		//		AllocationHeader* pOldAllHeader{ Get(m_UsedList, i)->Data };
		//		// AllocationHeader* pOldAllHeader{ *m_UsedList.At(i) };

		//		/* get our new free header */
		//		FreeHeader* pFreeHeader{ reinterpret_cast<FreeHeader*>(reinterpret_cast<uint64_t>(pNewStart) + totalAllocatedMem) };

		//		/* initialize new allocation header */
		//		AllocationHeader* pNewAllHeader{ reinterpret_cast<AllocationHeader*>(reinterpret_cast<uint64_t>(pFreeHeader) + sizeof(FreeHeader)) };
		//		pNewAllHeader->Size = pOldAllHeader->Size;
		//		pNewAllHeader->Padding = pOldAllHeader->Padding;

		//		/* copy over data */
		//		memcpy
		//		(
		//			reinterpret_cast<void*>(reinterpret_cast<uint64_t>(pNewAllHeader) + sizeof(AllocationHeader)),
		//			reinterpret_cast<void*>(reinterpret_cast<uint64_t>(pOldAllHeader) + sizeof(AllocationHeader)),
		//			pOldAllHeader->Size
		//		);

		//		/* make new free header from current free header */
		//		const uint64_t totalSize{ pOldAllHeader->Size + pOldAllHeader->Padding };
		//		FreeHeader* pNewFreeHeader{ reinterpret_cast<FreeHeader*>(reinterpret_cast<uint64_t>(pFreeHeader) + totalSize) };
		//		pNewFreeHeader->Size = newCap - totalAllocatedMem - totalSize;
		//		// pNewFreeHeader->Size = totalSize;

		//		/* keep track of how much memory we've currently copied over */
		//		totalAllocatedMem += totalSize;

		//		Remove(freeList, pFreeHeader);
		//		Add(freeList, pNewFreeHeader);
		//		Add(usedList, pNewAllHeader);
		//		// freeList.Erase(pFreeHeader);
		//		// freeList.Add(pNewFreeHeader);
		//		// usedList.Add(pNewAllHeader);
		//	}

		//	/* copium let's assume we have an element remaining, we should tho */
		//	/* I'm adding too much memory, because this can't be equal to newCap, especially if memory has been allocated */

		//	// Get(freeList, Count(freeList) - 1)->Data->Size = newCap - totalAllocatedMem;
		//	// (*freeList.Back())->Size = newCap - totalAllocatedMem;
		//}

		//__ASSERT(Count(usedList) == Count(m_UsedList));

		//free(m_FreeList);
		//free(m_UsedList);

		//m_FreeList = __MOVE(freeList);
		//m_UsedList = __MOVE(usedList);

		//free(m_pStart);
		//// delete[] m_pStart;

		//m_pStart = pNewStart;
		//m_Capacity = newCap;
	}
}
