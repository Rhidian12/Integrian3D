#pragma once

#include "../../EngineConstants.h"
#include "../AllocatorTraits/AllocatorTraits.h"

#include <vector> /* std::vector */

namespace Integrian3D::Memory
{
	template<typename T>
	class Allocator
	{
		using Traits = AllocatorTraits<T>;
	public:
		template<typename U>
		class Handle final
		{
		public:
			explicit constexpr Handle(T& alloc, const uint64_t memLoc)
				: m_Allocator{ alloc }
				, m_MemoryLocation{ memLoc }
			{}

#pragma region RuleOf5
			constexpr Handle(const Handle& other) noexcept
				: m_Allocator{ other.m_Allocator }
				, m_MemoryLocation{ other.m_MemoryLocation }
			{}
			constexpr Handle(Handle&& other) noexcept
				: m_Allocator{ __MOVE(T, other.m_Allocator) }
				, m_MemoryLocation{ __MOVE(uint64_t, other.m_MemoryLocation) }
			{}
			constexpr Handle& operator=(const Handle& other) noexcept
			{
				m_Allocator = other.m_Allocator;
				m_MemoryLocation = other.m_MemoryLocation;

				return *this;
			}
			constexpr Handle& operator=(Handle&& other) noexcept
			{
				m_Allocator = __MOVE(T, other.m_Allocator);
				m_MemoryLocation = __MOVE(uint64_t, other.m_MemoryLocation);

				return *this;
			}
#pragma endregion

#pragma region Pointer
			__NODISCARD __INLINE constexpr U* operator->()
			{
				return static_cast<U*>(m_Allocator.Get(m_MemoryLocation));
			}
			__NODISCARD __INLINE constexpr const U* operator->() const
			{
				return static_cast<U*>(m_Allocator.Get(m_MemoryLocation));
			}

			__NODISCARD __INLINE constexpr U& operator*()
			{
				return *static_cast<U*>(m_Allocator.Get(m_MemoryLocation));
			}
			__NODISCARD __INLINE constexpr const U& operator*() const
			{
				return *static_cast<U*>(m_Allocator.Get(m_MemoryLocation));
			}
#pragma endregion

		private:
			friend class Allocator;

			T& m_Allocator;
			uint64_t m_MemoryLocation;
		};

		constexpr explicit Allocator(const T& alloc = T{})
			: m_Allocator{ alloc }
			, m_TakenMemoryLocations{}
		{}

#pragma region RuleOf5
		constexpr Allocator(const Allocator& other) noexcept
			: m_Allocator{ other.m_Allocator }
			, m_TakenMemoryLocations{}
		{
			static_assert(Traits::IsCopyConstructable, "T::T(const T&) has not been defined");
		}
		constexpr Allocator(Allocator&& other) noexcept
			: m_Allocator{ __MOVE(T, other.m_Allocator) }
			, m_TakenMemoryLocations{ __MOVE(std::vector<uint64_t>, other.m_TakenMemoryLocations) }
		{
			static_assert(Traits::IsMoveConstructable, "T::T(T&&) has not been defined");

			other.m_TakenMemoryLocations.clear();
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

			m_Allocator = __MOVE(T, other.m_Allocator);
			m_TakenMemoryLocations = __MOVE(std::vector<uint64_t>, other.m_TakenMemoryLocations);

			other.m_TakenMemoryLocations.clear();

			return *this;
		}
#pragma endregion

		template<typename U>
		__NODISCARD constexpr Handle<U> Allocate(const uint64_t nrOfElements, const uint64_t align = alignof(U))
		{
			static_assert(Traits::template CanAllocate<U>, "T::Allocate() has not been defined");
			static_assert(Traits::HasGet, "T::Get() has not been defined");

#pragma warning ( push )
#pragma warning ( disable : 4834 ) // warning C4834: discarding return value of function with 'nodiscard' attribute
			m_Allocator.Allocate<U>(nrOfElements, align);
#pragma warning ( pop )

			uint64_t memLoc{};

			while (std::find(m_TakenMemoryLocations.cbegin(), m_TakenMemoryLocations.cend(), memLoc) != m_TakenMemoryLocations.cend())
			{
				++memLoc;
			}

			m_TakenMemoryLocations.push_back(memLoc);

			return Handle<U>{ m_Allocator, memLoc };
		}

		template<typename U>
		constexpr void Deallocate(Handle<U>& handle)
		{
			static_assert(Traits::CanDeallocate, "T::Deallocate() has not been defined");

			m_TakenMemoryLocations.erase
			(
				std::remove
				(
					m_TakenMemoryLocations.begin(),
					m_TakenMemoryLocations.end(),
					handle.m_MemoryLocation
				),
				m_TakenMemoryLocations.end()
			);

#pragma warning ( push )
#pragma warning ( disable : 4245 ) /* warning C4245: '=': conversion from 'int' to 'uint64_t', signed/unsigned mismatch */
			/* This should be completely invalid */
			handle.m_MemoryLocation = -1;
#pragma warning ( pop )
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

	private:
		T m_Allocator;
		std::vector<uint64_t> m_TakenMemoryLocations;
	};
}