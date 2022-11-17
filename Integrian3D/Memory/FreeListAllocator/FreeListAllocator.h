#pragma once

#include "../../EngineConstants.h"
#include "../MemoryUtils.h"

namespace Integrian3D::Memory
{
	class FreeListAllocator final
	{
		struct Block final
		{
			uint64_t Size;
			Block* pNext;
		};

		struct Header final
		{
			uint64_t Size;
			uint64_t Adjustment;
		};

	public:
		using CanMove = std::true_type;
		using CanCopy = std::true_type;

		FreeListAllocator();
		explicit FreeListAllocator(const uint64_t size);

		~FreeListAllocator();

		FreeListAllocator(const FreeListAllocator& other) noexcept;
		FreeListAllocator(FreeListAllocator&& other) noexcept;
		FreeListAllocator& operator=(const FreeListAllocator& other) noexcept;
		FreeListAllocator& operator=(FreeListAllocator&& other) noexcept;

		template<typename T>
		__NODISCARD constexpr T* Allocate(const uint64_t nrOfElements, const size_t align = alignof(T))
		{
			__ASSERT(nrOfElements != 0 && "FreeListAllocators::Allocate() > Cannot allocate 0 elements");

			Block* pPreviousBlock = nullptr;
			Block* pFreeBlock = m_pFreeBlocks;

			Block* pPreviousBestFit = nullptr;
			Block* pBestFit = nullptr;

			size_t bestFitAdjustment = 0;
			size_t bestFitTotalSize = 0;

			while (pFreeBlock)
			{
				/* Alignment adjustment needed to store the Header */
				bestFitAdjustment = AlignForward<Header, Block>(pFreeBlock, align);

				/* Calculate total size */
				bestFitTotalSize = nrOfElements * sizeof(T) + bestFitAdjustment;

				/* Is the current block a better fit than the current best fit? */
				if (pFreeBlock->Size >= bestFitTotalSize && (!pBestFit || pFreeBlock->Size < pBestFit->Size))
				{
					pPreviousBestFit = pPreviousBlock;
					pBestFit = pFreeBlock;

					/* Is the new current block a perfect fit? */
					if (pFreeBlock->Size == bestFitTotalSize)
					{
						break;
					}
				}

				pPreviousBlock = pFreeBlock;
				pFreeBlock = pFreeBlock->pNext;
			}

			// __ASSERT(pBestFit != nullptr && "FreeListAllocator::Allocate() > Ran out of memory");

			if (!pBestFit)
			{
				Reallocate<T>(nrOfElements);

				return Allocate<T>(nrOfElements, align);
			}

			/* Can the best fit be split into two parts? */
			if (pBestFit->Size - bestFitTotalSize > sizeof(Header))
			{
				/* Split the block into memory for the allocation and the remainder */

				/* New block starts at best fit + size */
				Block* const pNewBlock{ reinterpret_cast<Block*>(reinterpret_cast<size_t>(pBestFit) + bestFitTotalSize) };

				/* The new block has the remaining size */
				pNewBlock->Size = pBestFit->Size - bestFitTotalSize;
				/* The new block now points to the original block's next */
				pNewBlock->pNext = pBestFit->pNext;

				/* Instert the new block into the list */
				/* By not pointing towards pNewBlock instead of pBestFit, we're removing it from the list */
				if (pPreviousBestFit)
				{
					pPreviousBestFit->pNext = pNewBlock;
				}
				else
				{
					m_pFreeBlocks = pNewBlock;
				}
			}
			else /* we can't split it into two */
			{
				/* Update the size to the entire best fit block size */
				bestFitTotalSize = pBestFit->Size;

				/* Remove pBestFit from the linked list */
				if (pPreviousBestFit)
				{
					pPreviousBestFit->pNext = pBestFit->pNext;
				}
				else
				{
					m_pFreeBlocks = pBestFit->pNext;
				}
			}

			/* Get the aligned address */
			const size_t alignedAddress{ reinterpret_cast<size_t>(pBestFit) + bestFitAdjustment };

			/* Get the header from this aligned address */
			Header* const pHeader{ reinterpret_cast<Header*>(alignedAddress - sizeof(Header)) };

			pHeader->Size = bestFitTotalSize;
			pHeader->Adjustment = bestFitAdjustment;

			++m_Size;

			return reinterpret_cast<T*>(alignedAddress);
		}

		constexpr void Deallocate(void* p)
		{
			if (!p)
			{
				return;
			}

			/* Get the header from the memory we allocated */
			Header* const pHeader{ reinterpret_cast<Header*>(reinterpret_cast<size_t>(p) - sizeof(Header)) };

			/* Get the actual start of the allocation by moving backwards the amount specified by the header */
			const size_t blockStart{ reinterpret_cast<size_t>(p) - pHeader->Adjustment };
			const size_t blockSize{ pHeader->Size };
			const size_t blockEnd{ blockStart + blockSize };

			Block* pPreviousFreeBlock{};
			Block* pFreeBlock{ m_pFreeBlocks };

			/* Find the first block that starts after this heap of allocated memory */
			while (pFreeBlock)
			{
				if (reinterpret_cast<size_t>(pFreeBlock) >= blockEnd)
				{
					break;
				}

				pPreviousFreeBlock = pFreeBlock;
				pFreeBlock = pFreeBlock->pNext;
			}

			if (!pPreviousFreeBlock)
			{
				/* There is no free block after this heap of allocated memory, so add it to the start of the list */
				pPreviousFreeBlock = reinterpret_cast<Block*>(blockStart);
				pPreviousFreeBlock->Size = blockSize;
				pPreviousFreeBlock->pNext = m_pFreeBlocks;

				m_pFreeBlocks = pPreviousFreeBlock;
			}
			else if (reinterpret_cast<size_t>(pPreviousFreeBlock) + pPreviousFreeBlock->Size == blockStart)
			{
				/* The block before (pPreviousFreeBlock) the block we're freeing (pFreeBlock) ends right on the boundary of the heap of allocated memory */
				/* So we can merge the previous block and our heap of allocated memory together */

				pPreviousFreeBlock->Size += blockSize;
			}
			else
			{
				/* The block before (pPreviousBlock) the block we're freeing (pFreeBlock) does NOT end rightt on the boundary of the heap of allocated memory */
				/* Therefore we cannot merge the two */
				/* So, we have to create a new Block */

				Block* const pTemp{ reinterpret_cast<Block*>(blockStart) };

				pTemp->Size = blockSize;
				pTemp->pNext = pPreviousFreeBlock->pNext;

				pPreviousFreeBlock->pNext = pTemp;
				pPreviousFreeBlock = pTemp;
			}

			if (reinterpret_cast<size_t>(pPreviousFreeBlock) + pPreviousFreeBlock->Size ==
				reinterpret_cast<size_t>(pPreviousFreeBlock->pNext))
			{
				/* The new or merged block ends right on the next block of list, so we can merge these again */
				pPreviousFreeBlock->Size += pPreviousFreeBlock->pNext->Size;
				pPreviousFreeBlock->pNext = pPreviousFreeBlock->pNext->pNext;
			}

			--m_Size;
		}

		__NODISCARD constexpr uint64_t Capacity() const { return m_Capacity; }
		__NODISCARD constexpr uint64_t Size() const { return m_Size; }
		__NODISCARD constexpr uint64_t MaxSize() const { return std::numeric_limits<uint64_t>::max(); }
		__NODISCARD constexpr const void* Data() const { return m_pStart; }


	private:
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

		template<typename T>
		constexpr void Reallocate(const uint64_t nrOfElements)
		{
			const uint64_t newCap{ CalculateNewCapacity(m_Capacity + sizeof(T) * nrOfElements + sizeof(Header) + sizeof(Block) + 1) };
			Block* const pNewStart{ static_cast<Block*>(malloc(newCap)) };

			Block* pCurrentBlock{ static_cast<Block*>(m_pStart) };
			Block* pNewCurrentBlock{ pNewStart };

			uint64_t sizeToReallocate{ newCap };

			while (pCurrentBlock)
			{
				/* Copy over Block size */
				pNewCurrentBlock->Size = pCurrentBlock->Size;

				/* Set the next to a nullptr by default */
				pNewCurrentBlock->pNext = nullptr;

				/* Copy over Header information */
				const uint64_t oldDataAddress{ reinterpret_cast<uint64_t>(pCurrentBlock) +
					AlignForward<Header, Block>(pCurrentBlock, alignof(Block)) };
				const uint64_t newDataAddress{ reinterpret_cast<uint64_t>(pNewCurrentBlock) +
					AlignForward<Header, Block>(pNewCurrentBlock, alignof(Block)) };

				Header* const pOldHeader{ reinterpret_cast<Header*>(oldDataAddress - sizeof(Header)) };
				Header* const pNewHeader{ reinterpret_cast<Header*>(newDataAddress - sizeof(Header)) };

				pNewHeader->Adjustment = pOldHeader->Adjustment;
				pNewHeader->Size = pOldHeader->Size;

				/* Move over the old data */
				memmove
				(
					reinterpret_cast<void*>(newDataAddress),
					reinterpret_cast<void*>(oldDataAddress),
					pCurrentBlock->Size - sizeof(Header) + sizeof(Block)
				);

				/* decrease how much size we still have to reallocate */
				sizeToReallocate -= pCurrentBlock->Size;

				/* If there is a next entry in the list, add it to the new one */
				if (sizeToReallocate > 0 && !pCurrentBlock->pNext)
				{
					Block* const pNewBlock{ reinterpret_cast<Block*>(reinterpret_cast<uint64_t>(pNewCurrentBlock) + pNewCurrentBlock->Size) };
					pNewCurrentBlock->pNext = pNewBlock;
					pNewCurrentBlock->Size = sizeToReallocate;

					sizeToReallocate = 0;
				}

				pCurrentBlock = pCurrentBlock->pNext;
				pNewCurrentBlock = pNewCurrentBlock->pNext;
			}

			delete[] m_pStart;

			m_pStart = pNewStart;
			m_pFreeBlocks = pNewStart;
			m_Capacity = newCap;
		}

		void* m_pStart;

		Block* m_pFreeBlocks;
		uint64_t m_Size;
		uint64_t m_Capacity;
	};
}