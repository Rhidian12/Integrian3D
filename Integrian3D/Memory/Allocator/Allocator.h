#pragma once

#include "../../EngineConstants.h"
#include "../AllocatorTraits/AllocatorTraits.h"

#include <vector> /* std::vector */
#include <memory> /* std::unique_ptr */

namespace Integrian3D::Memory
{
	template<typename T>
	class Allocator
	{
		using Traits = AllocatorTraits<T>;

	public:
		constexpr explicit Allocator(const T& alloc = T{})
			: m_Allocator{ alloc }
		{
			if constexpr (Traits::HasInitialize)
				m_Allocator.Initialize();
		}
		constexpr ~Allocator() = default;

#pragma region RuleOf5
		constexpr Allocator(const Allocator& other) noexcept
			: m_Allocator{ other.m_Allocator }
		{
			static_assert(Traits::IsCopyConstructable, "T::T(const T&) has not been defined");
		}
		constexpr Allocator(Allocator&& other) noexcept
			: m_Allocator{ I_MOVE(other.m_Allocator) }
		{
			static_assert(Traits::IsMoveConstructable, "T::T(T&&) has not been defined");
		}
		constexpr Allocator& operator=(const Allocator& other) noexcept
		{
			static_assert(Traits::IsCopyAssignable, "T& T::operator=(const T&) has not been defined");

			m_Allocator = other.m_Allocator;

			return *this;
		}
		constexpr Allocator& operator=(Allocator&& other) noexcept
		{
			static_assert(Traits::IsMoveAssignable, "T& T::operator=(T&&) has not been defined");

			m_Allocator = I_MOVE(other.m_Allocator);

			return *this;
		}
#pragma endregion

		template<typename U>
		__NODISCARD constexpr U* Allocate(const uint64_t nrOfElements, const uint64_t align = alignof(U))
		{
			static_assert(Traits::CanAllocate, "T::Allocate() has not been defined");

			return static_cast<U*>(m_Allocator.Allocate(nrOfElements * sizeof(U), align));
		}

		template<typename U>
		constexpr void Deallocate(U* pHandle)
		{
			static_assert(Traits::CanDeallocate, "void T::Deallocate() has not been defined");
			static_assert(Traits::template CanDestroy<U>, "void T::Destroy() has not been defined");

			m_Allocator.Destroy(pHandle);
			m_Allocator.Deallocate(pHandle);
		}

		template<typename U>
		constexpr void Destroy(U* p) const
		{
			static_assert(Traits::template CanDestroy<U>, "void T::Destroy() has not been defined");

			m_Allocator.Destroy(p);
		}

		__NODISCARD __INLINE constexpr uint64_t Capacity() const
		{
			static_assert(Traits::HasCapacity, "uint64_t T::Capacity() has not been defined");

			return m_Allocator.Capacity();
		}
		__NODISCARD __INLINE constexpr uint64_t Size() const
		{
			static_assert(Traits::HasSize, "uint64_t T::Size() has not been defined");

			return m_Allocator.Size();
		}

		__NODISCARD constexpr T& GetAllocator()
		{
			return m_Allocator;
		}
		__NODISCARD constexpr const T& GetAllocator() const
		{
			return m_Allocator;
		}

	private:
		T m_Allocator;
	};
}