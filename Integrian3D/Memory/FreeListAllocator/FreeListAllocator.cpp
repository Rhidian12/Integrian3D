#include "FreeListAllocator.h"

namespace Integrian3D::Memory
{
	FreeListAllocator::FreeListAllocator(const PlacementPolicy policy)
		: FreeListAllocator{ 1024, policy }
	{}
	FreeListAllocator::FreeListAllocator(const uint64_t size, const PlacementPolicy policy)
		: m_pStart{}
		, m_PlacementPolicy{ policy }
		, m_FreeList{}
		, m_UsedList{}
		, m_Size{}
		, m_Capacity{ size }
	{
		m_pStart = new char[size] {};

		m_FreeList.Add(static_cast<FreeHeader*>(m_pStart));
		(*m_FreeList.Front())->Size = size;
	}

	FreeListAllocator::~FreeListAllocator()
	{
		delete[] m_pStart;
		m_pStart = nullptr;
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
		m_pStart = new char[other.m_Capacity]{};

		m_FreeList.Add(static_cast<FreeHeader*>(m_pStart));
		(*m_FreeList.Front())->Size = m_Capacity;
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
		other.m_FreeList.Clear();
		other.m_UsedList.Clear();
	}

	FreeListAllocator& FreeListAllocator::operator=(const FreeListAllocator& other) noexcept
	{
		if (m_pStart)
		{
			delete[] m_pStart;
			m_pStart = nullptr;
		}

		m_PlacementPolicy = other.m_PlacementPolicy;
		m_Capacity = other.m_Capacity;

		m_pStart = new char[other.m_Capacity]{};

		m_FreeList.Add(static_cast<FreeHeader*>(m_pStart));
		(*m_FreeList.Front())->Size = m_Capacity;

		return *this;
	}

	FreeListAllocator& FreeListAllocator::operator=(FreeListAllocator&& other) noexcept
	{
		if (m_pStart)
		{
			delete[] m_pStart;
			m_pStart = nullptr;
		}

		m_pStart = __MOVE(other.m_pStart);
		m_PlacementPolicy = __MOVE(other.m_PlacementPolicy);
		m_FreeList = __MOVE(other.m_FreeList);
		m_UsedList = __MOVE(other.m_UsedList);
		m_Size = __MOVE(other.m_Size);
		m_Capacity = __MOVE(other.m_Capacity);

		other.m_pStart = nullptr;
		other.m_FreeList.Clear();
		other.m_UsedList.Clear();

		return *this;
	}
#pragma endregion
}
