#pragma once

#include "../../EngineConstants.h"
#include "../AllocatorTraits/AllocatorTraits.h"

namespace Integrian3D::Memory
{
	template<typename T>
	class Allocator
	{
	public:
		constexpr explicit Allocator(const T& alloc = T{})
			: m_Allocator{ alloc }
		{}

		template<typename U>
		constexpr void Allocate(const uint64_t nrOfElements, const uint64_t align = alignof(U))
		{
			static_assert(AllocatorTraits<T>::CanAllocate, "T::Allocate() has not been defined");
		}

	private:
		T m_Allocator;
	};
}