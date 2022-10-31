#include "FreeListAllocator.h"

namespace Integrian3D::Memory
{
	FreeListAllocator::FreeListAllocator()
		: FreeListAllocator{ 1024 }
	{}
	FreeListAllocator::FreeListAllocator(const uint64_t size)
		: m_pStart{}
		, m_pFreeBlocks{}
		, m_Size{}
		, m_Capacity{ size }
	{
		m_pStart = malloc(size);
		m_pFreeBlocks = static_cast<Block*>(m_pStart);

		m_pFreeBlocks->pNext = nullptr;
		m_pFreeBlocks->Size = size;
	}

	FreeListAllocator::~FreeListAllocator()
	{
		free(m_pStart);
	}

	FreeListAllocator::FreeListAllocator(const FreeListAllocator& other) noexcept
		: m_pStart{}
		, m_pFreeBlocks{}
		, m_Size{ other.m_Size }
		, m_Capacity{ other.m_Capacity }
	{
		m_pStart = malloc(m_Size);
		m_pFreeBlocks = static_cast<Block*>(m_pStart);

		m_pFreeBlocks->pNext = nullptr;
		m_pFreeBlocks->Size = m_Size;
	}

	FreeListAllocator::FreeListAllocator(FreeListAllocator&& other) noexcept
		: m_pStart{ __MOVE(void*, other.m_pStart) }
		, m_pFreeBlocks{ __MOVE(Block*, other.m_pFreeBlocks) }
		, m_Size{ __MOVE(uint64_t, other.m_Size) }
		, m_Capacity{ __MOVE(uint64_t,other.m_Capacity) }
	{
		other.m_pStart = nullptr;
		other.m_pFreeBlocks = nullptr;
		other.m_Size = 0;
		other.m_Capacity = 0;
	}

	FreeListAllocator& FreeListAllocator::operator=(const FreeListAllocator& other) noexcept
	{
		if (m_pStart)
		{
			free(m_pStart);
		}

		m_Size = other.m_Size;
		m_Capacity = other.m_Capacity;

		m_pStart = malloc(m_Size);
		m_pFreeBlocks = static_cast<Block*>(m_pStart);

		m_pFreeBlocks->pNext = nullptr;
		m_pFreeBlocks->Size = m_Size;

		return *this;
	}

	FreeListAllocator& FreeListAllocator::operator=(FreeListAllocator&& other) noexcept
	{
		if (m_pStart)
		{
			free(m_pStart);
		}

		m_pStart = __MOVE(void*, other.m_pStart);
		m_pFreeBlocks = __MOVE(Block*, other.m_pFreeBlocks);
		m_Size = __MOVE(uint64_t, other.m_Size);
		m_Capacity = __MOVE(uint64_t, other.m_Capacity);

		other.m_pStart = nullptr;
		other.m_pFreeBlocks = nullptr;
		other.m_Size = 0;
		other.m_Capacity = 0;

		return *this;
	}
}
