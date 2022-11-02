#pragma once

#include "../../EngineConstants.h"

namespace Integrian3D::Memory
{
	class MemoryTracker final
	{
	public:
		__NODISCARD __INLINE static constexpr void Track(const size_t n)
		{
			m_AllocatedMemory += n;
		}

		__NODISCARD __INLINE static constexpr void StopTracking(const size_t n)
		{
			m_AllocatedMemory -= n;
		}

		__NODISCARD __INLINE static constexpr uint64_t GetTotalAllocatedMemory()
		{
			return m_AllocatedMemory;
		}

	private:
		MemoryTracker() = default;

		inline static uint64_t m_AllocatedMemory{};
	};
}