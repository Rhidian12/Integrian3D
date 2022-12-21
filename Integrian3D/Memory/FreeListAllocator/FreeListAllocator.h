#pragma once

#include "../../EngineConstants.h"
#include "../MemoryUtils.h"
#include "../../LinkedList/LinkedList.h"

#include <iostream>

namespace Integrian3D::Memory
{
	class FreeListAllocator final
	{
	public:
		enum class PlacementPolicy
		{
			FIND_FIRST_FIT,
			FIND_BEST_FIT
		};

	private:
		/* [CRINGE]: Do we really need 2 seperate structs for this kind of stuff? Could we not re-use the FreeHeader? */
		struct FreeHeader final
		{
			uint64_t Size;
		};

		/* [CRINGE]: Is this Padding necessary? Could we not just keep track of the entire Size instead of splitting it up? */
		struct AllocationHeader final
		{
			uint64_t Size;
			uint64_t Padding;
		};

		inline constexpr static uint64_t m_NodePadding{ sizeof(FreeHeader) + sizeof(AllocationHeader) + sizeof(FreeHeader*) };

	public:
		FreeListAllocator(const PlacementPolicy policy = PlacementPolicy::FIND_FIRST_FIT);
		explicit FreeListAllocator(const uint64_t size, const PlacementPolicy policy = PlacementPolicy::FIND_FIRST_FIT);

		~FreeListAllocator();

		FreeListAllocator(const FreeListAllocator& other) noexcept;
		FreeListAllocator(FreeListAllocator&& other) noexcept;
		FreeListAllocator& operator=(const FreeListAllocator& other) noexcept;
		FreeListAllocator& operator=(FreeListAllocator&& other) noexcept;

		template<typename T>
		__NODISCARD T* Allocate(const uint64_t n, const uint64_t alignment)
		{
			__ASSERT(n > 0 && "Allocation size must be bigger");

			const uint64_t allocationSize{ n * sizeof(T) };

			// Search through the free list for a free block that has enough space to allocate our data
			// padding takes alignment into account
			uint64_t padding{};
			FreeHeader* pFreeHeader{};
			Find(allocationSize, alignment, padding, pFreeHeader);

			const uint64_t totalAllocationSize{ allocationSize + padding };

			if (!pFreeHeader)
			{
				Reallocate(totalAllocationSize + sizeof(FreeHeader*));

				return Allocate<T>(n, alignment);
			}

			const uint64_t rest{ pFreeHeader->Size - totalAllocationSize };

			if (rest > 0)
			{
				// We have to split the block into the data block and a free block of size 'rest'
				FreeHeader* pNewNode{ reinterpret_cast<FreeHeader*>(reinterpret_cast<uint64_t>(pFreeHeader) + totalAllocationSize) };
				pNewNode->Size = rest;

				m_FreeList.Add(pNewNode);
			}

			m_FreeList.Erase(pFreeHeader);

			/* Set up data */
			AllocationHeader* pAllHeader{ reinterpret_cast<AllocationHeader*>(reinterpret_cast<uint64_t>(pFreeHeader) + sizeof(FreeHeader)) };
			pAllHeader->Size = allocationSize;
			pAllHeader->Padding = padding;

			m_UsedList.Add(pAllHeader);

			++m_Size;

			return reinterpret_cast<T*>(reinterpret_cast<uint64_t>(pAllHeader) + sizeof(AllocationHeader));
		}

		void Deallocate(void* p)
		{
			if (!p)
				return;

			AllocationHeader* pAllHeader{ reinterpret_cast<AllocationHeader*>(reinterpret_cast<uint64_t>(p) - sizeof(AllocationHeader)) };
			FreeHeader* pFreeHeader{ reinterpret_cast<FreeHeader*>(reinterpret_cast<uint64_t>(pAllHeader) - sizeof(FreeHeader)) };
			pFreeHeader->Size = pAllHeader->Size + pAllHeader->Padding;

			m_UsedList.Erase(pAllHeader);

			--m_Size;

			FreeHeader* pLastBlock{};

			const uint64_t nrOfFreeHeaders{ m_FreeList.Size() };
			if (nrOfFreeHeaders > 0u)
				pLastBlock = *m_FreeList.Back();

			if (nrOfFreeHeaders == 0u)
				m_FreeList.Add(pFreeHeader);
			else if (reinterpret_cast<uint64_t>(pLastBlock) + pLastBlock->Size == reinterpret_cast<uint64_t>(pFreeHeader))
				pLastBlock->Size += pFreeHeader->Size;
			else
				m_FreeList.Add(pFreeHeader);

			if (pLastBlock)
			{
				FreeHeader* pNewLastBlock{ *m_FreeList.Back() };

				if (reinterpret_cast<uint64_t>(pLastBlock) + pLastBlock->Size == reinterpret_cast<uint64_t>(pNewLastBlock))
				{
					pLastBlock->Size += pNewLastBlock->Size;
					m_FreeList.Erase(pNewLastBlock);
				}
			}
		}

		template<typename T>
		constexpr void Destroy(T* p) const
		{
			if (!p)
				return;

			p->~T();
		}

		__NODISCARD void* Get(const uint64_t n) const
		{
			if (n >= m_Size)
				return nullptr;

			return reinterpret_cast<void*>(reinterpret_cast<uint64_t>(*m_UsedList.At(n)) + sizeof(AllocationHeader));
		}

		__NODISCARD uint64_t GetAllocationSize(const uint64_t n) const
		{
			if (n >= m_Size)
				return 0u;

			return (*m_UsedList.At(n))->Size;
		}

		__NODISCARD constexpr uint64_t Capacity() const
		{
			return m_Capacity;
		}
		__NODISCARD constexpr uint64_t Size() const
		{
			return m_Size;
		}
		__NODISCARD constexpr uint64_t MaxSize() const
		{
			return std::numeric_limits<uint64_t>::max();
		}
		__NODISCARD constexpr void* Data() const
		{
			return m_pStart;
		}

	private:
		constexpr void Find(const uint64_t size, const uint64_t alignment, uint64_t& padding, FreeHeader*& pFreeHeader)
		{
			switch (m_PlacementPolicy)
			{
			case PlacementPolicy::FIND_FIRST_FIT:
				FindFirst(size, alignment, padding, pFreeHeader);
				break;
			case PlacementPolicy::FIND_BEST_FIT:
				FindBest(size, alignment, padding, pFreeHeader);
				break;
			}
		}

		void FindFirst(const uint64_t size, const uint64_t alignment, uint64_t& padding, FreeHeader*& pFreeHeader)
		{
			/* [TODO]: Use iterators */
			for (uint64_t i{}; i < m_FreeList.Size(); ++i)
			{
				FreeHeader* pHeader{ *m_FreeList.At(i) };

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

		void FindBest(const uint64_t size, const uint64_t alignment, uint64_t& padding, FreeHeader*& pFreeHeader)
		{
			/* [TODO]: Use iterators */
			FreeHeader* pBestFit{};
			for (uint64_t i{}; i < m_FreeList.Size(); ++i)
			{
				FreeHeader* pHeader{ *m_FreeList.At(i) };

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

		void Reallocate(const uint64_t requiredSize)
		{
			const uint64_t newCap{ CalculateNewCapacity(m_Capacity + requiredSize + m_NodePadding + 1u) };
			FreeHeader* const pNewStart{ reinterpret_cast<FreeHeader*>(new char[newCap] {}) };
			pNewStart->Size = newCap;

			LinkedList<FreeHeader*> freeList{};
			LinkedList<AllocationHeader*> usedList{};
			freeList.Add(pNewStart);
			uint64_t totalAllocatedMem{};

			if (m_UsedList.Size() > 0u)
			{
				/* loop over every allocated piece of memory and copy it over */
				for (uint64_t i{}; i < m_UsedList.Size(); ++i)
				{
					/* get currently allocated memory */
					AllocationHeader* pOldAllHeader{ *m_UsedList.At(i) };

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

					freeList.Erase(pFreeHeader);
					freeList.Add(pNewFreeHeader);
					usedList.Add(pNewAllHeader);
				}

				/* copium let's assume we have an element remaining, we should tho */
				/* I'm adding too much memory, because this can't be equal to newCap, especially if memory has been allocated */

				(*freeList.Back())->Size = newCap - totalAllocatedMem;
			}

			m_FreeList = __MOVE(freeList);
			m_UsedList = __MOVE(usedList);

			delete[] m_pStart;

			m_pStart = pNewStart;
			m_Capacity = newCap;
		}

		void* m_pStart;
		PlacementPolicy m_PlacementPolicy;
		LinkedList<FreeHeader*> m_FreeList;
		LinkedList<AllocationHeader*> m_UsedList;
		uint64_t m_Size;
		uint64_t m_Capacity;
	};
}