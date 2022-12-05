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

		class IHandle
		{
		public:
			constexpr IHandle(const uint64_t memLoc)
				: m_MemoryLocation{ memLoc }
			{}
			constexpr virtual ~IHandle() = default;

#pragma region RuleOf5
			IHandle(const IHandle& other) noexcept
				: m_MemoryLocation{ other.m_MemoryLocation }
			{}
			IHandle(IHandle&& other) noexcept
				: m_MemoryLocation{ __MOVE(other.m_MemoryLocation) }
			{}
			IHandle& operator=(const IHandle& other) noexcept
			{
				m_MemoryLocation = other.m_MemoryLocation;

				return *this;
			}
			IHandle& operator=(IHandle&& other) noexcept
			{
				m_MemoryLocation = __MOVE(other.m_MemoryLocation);

				return *this;
			}
#pragma endregion

			__NODISCARD constexpr uint64_t GetMemoryLocation() const
			{
				return m_MemoryLocation;
			}

		protected:
			friend class Allocator;

			uint64_t m_MemoryLocation;
		};
	public:
		template<typename U>
		class Handle final : public IHandle
		{
		public:
			explicit constexpr Handle(T& alloc, const uint64_t memLoc)
				: IHandle{ memLoc }
				, m_Allocator{ alloc }
			{}

#pragma region RuleOf5
			constexpr Handle(const Handle& other) noexcept
				: IHandle{ other }
				, m_Allocator{ other.m_Allocator }
			{}
			constexpr Handle(Handle&& other) noexcept
				: IHandle{ __MOVE(other) }
				, m_Allocator{ __MOVE(other.m_Allocator) }
			{}
			constexpr Handle& operator=(const Handle& other) noexcept
			{
				IHandle::operator=(other);
				m_Allocator = other.m_Allocator;

				return *this;
			}
			constexpr Handle& operator=(Handle&& other) noexcept
			{
				IHandle::operator=(__MOVE(other));
				m_Allocator = __MOVE(other.m_Allocator);

				return *this;
			}
#pragma endregion

#pragma region Pointer
			__NODISCARD __INLINE constexpr U* operator->()
			{
				return static_cast<U*>(m_Allocator.Get(this->m_MemoryLocation));
			}
			__NODISCARD __INLINE constexpr const U* operator->() const
			{
				return static_cast<U*>(m_Allocator.Get(this->m_MemoryLocation));
			}

			__NODISCARD __INLINE constexpr U& operator*()
			{
				return *static_cast<U*>(m_Allocator.Get(this->m_MemoryLocation));
			}
			__NODISCARD __INLINE constexpr const U& operator*() const
			{
				return *static_cast<U*>(m_Allocator.Get(this->m_MemoryLocation));
			}
#pragma endregion

#pragma region MemoryInfo
			__NODISCARD constexpr uint64_t Size() const
			{
				return m_Allocator.GetAllocationSize(this->m_MemoryLocation);
			}
#pragma endregion

		private:
			T& m_Allocator;
		};

		constexpr explicit Allocator(const T& alloc = T{})
			: m_Allocator{ alloc }
			, m_TakenMemoryLocations{}
		{}
		constexpr ~Allocator()
		{
			for (IHandle* const pHandle : m_Handles)
				delete pHandle;
		}

#pragma region RuleOf5
		constexpr Allocator(const Allocator& other) noexcept
			: m_Allocator{ other.m_Allocator }
			, m_TakenMemoryLocations{}
		{
			static_assert(Traits::IsCopyConstructable, "T::T(const T&) has not been defined");
		}
		constexpr Allocator(Allocator&& other) noexcept
			: m_Allocator{ __MOVE(other.m_Allocator) }
			, m_TakenMemoryLocations{ __MOVE(other.m_TakenMemoryLocations) }
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

			m_Allocator = __MOVE(other.m_Allocator);
			m_TakenMemoryLocations = __MOVE(other.m_TakenMemoryLocations);

			other.m_TakenMemoryLocations.clear();

			return *this;
		}
#pragma endregion

		template<typename U>
		__NODISCARD constexpr Handle<U>& Allocate(const uint64_t nrOfElements, const uint64_t align = alignof(U))
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

			Handle<U>* pHandle{ new Handle<U>{ m_Allocator, memLoc } };
			m_Handles.push_back(pHandle);

			return *pHandle;
		}

		template<typename U>
		constexpr void Deallocate(Handle<U>& handle)
		{
			static_assert(Traits::CanDeallocate, "T::Deallocate() has not been defined");

			m_Allocator.Deallocate(&(*handle));

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

			auto it
			{
				std::find_if
					(
						m_Handles.cbegin(), m_Handles.cend(),
						[&handle](const IHandle* const pHandle)->bool
						{
							return handle.m_MemoryLocation == pHandle->m_MemoryLocation;
						}
					)
			};

			for (IHandle* pHandle : m_Handles)
				if (pHandle->m_MemoryLocation > handle.m_MemoryLocation)
					--pHandle->m_MemoryLocation;

			for (uint64_t& memLoc : m_TakenMemoryLocations)
				if (memLoc > handle.m_MemoryLocation)
					--memLoc;

			delete (*it);
			m_Handles.erase(it);
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
		std::vector<IHandle*> m_Handles;
	};
}