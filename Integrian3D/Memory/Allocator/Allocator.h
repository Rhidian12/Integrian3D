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
			constexpr IHandle(T* pAlloc, const uint64_t memLoc)
				: m_MemoryLocation{ memLoc }
				, m_IsValid{ false }
				, m_pAllocator{ pAlloc }
			{}
			constexpr virtual ~IHandle() = default;

#pragma region RuleOf5
			IHandle(const IHandle& other) noexcept
				: m_MemoryLocation{ other.m_MemoryLocation }
				, m_IsValid{ other.m_IsValid }
				, m_pAllocator{ other.m_pAllocator }
			{}
			IHandle(IHandle&& other) noexcept
				: m_MemoryLocation{ __MOVE(other.m_MemoryLocation) }
				, m_IsValid{ __MOVE(other.m_IsValid) }
				, m_pAllocator{ __MOVE(other.m_pAllocator) }
			{}
			IHandle& operator=(const IHandle& other) noexcept
			{
				m_MemoryLocation = other.m_MemoryLocation;
				m_IsValid = other.m_IsValid;
				m_pAllocator = other.m_pAllocator;

				return *this;
			}
			IHandle& operator=(IHandle&& other) noexcept
			{
				m_MemoryLocation = __MOVE(other.m_MemoryLocation);
				m_IsValid = __MOVE(other.m_IsValid);
				m_pAllocator = __MOVE(other.m_pAllocator);

				return *this;
			}
#pragma endregion

			__NODISCARD constexpr uint64_t GetMemoryLocation() const
			{
				return m_MemoryLocation;
			}
			__NODISCARD constexpr bool IsValid() const
			{
				return m_IsValid;
			}
			__NODISCARD constexpr uint64_t Size() const
			{
				if (!m_IsValid)
					return 0u;

				return m_pAllocator->GetAllocationSize(m_MemoryLocation);
			}

		protected:
			friend class Allocator;

			uint64_t m_MemoryLocation;
			bool m_IsValid;
			T* m_pAllocator;
		};
	public:
		template<typename U>
		class Handle final : public IHandle
		{
		public:
			explicit constexpr Handle(T* pAlloc, const uint64_t memLoc)
				: IHandle{ pAlloc, memLoc }
			{}

#pragma region RuleOf5
			constexpr Handle(const Handle& other) noexcept
				: IHandle{ other }
			{}
			constexpr Handle(Handle&& other) noexcept
				: IHandle{ __MOVE(other) }
			{}
			constexpr Handle& operator=(const Handle& other) noexcept
			{
				IHandle::operator=(other);

				return *this;
			}
			constexpr Handle& operator=(Handle&& other) noexcept
			{
				IHandle::operator=(__MOVE(other));

				return *this;
			}
#pragma endregion

#pragma region Pointer
			__NODISCARD __INLINE constexpr U* operator->()
			{
				if (!this->m_IsValid)
					return nullptr;

				return static_cast<U*>(this->m_pAllocator->Get(this->m_MemoryLocation));
			}
			__NODISCARD __INLINE constexpr const U* operator->() const
			{
				if (!this->m_IsValid)
					return nullptr;

				return static_cast<U*>(this->m_pAllocator->Get(this->m_MemoryLocation));
			}

			__NODISCARD __INLINE constexpr U& operator*()
			{
				__ASSERT(this->m_IsValid);

				return *static_cast<U*>(this->m_pAllocator->Get(this->m_MemoryLocation));
			}
			__NODISCARD __INLINE constexpr const U& operator*() const
			{
				__ASSERT(this->m_IsValid);

				return *static_cast<U*>(this->m_pAllocator->Get(this->m_MemoryLocation));
			}

			__NODISCARD __INLINE constexpr U* operator&()
			{
				if (!this->m_IsValid)
					return nullptr;

				return static_cast<U*>(this->m_pAllocator->Get(this->m_MemoryLocation));
			}
			__NODISCARD __INLINE constexpr const U* operator&() const
			{
				if (!this->m_IsValid)
					return nullptr;

				return static_cast<U*>(this->m_pAllocator->Get(this->m_MemoryLocation));
			}
#pragma endregion
		};

		constexpr explicit Allocator(const T& alloc = T{})
			: m_Allocator{ alloc }
			, m_TakenMemoryLocations{}
		{}
		constexpr ~Allocator()
		{
			for (IHandle* const pHandle : m_Handles)
				delete pHandle;

			m_Handles.clear();
		}

#pragma region RuleOf5
		constexpr Allocator(const Allocator& other) noexcept
			: m_Allocator{ other.m_Allocator }
			, m_TakenMemoryLocations{}
			, m_Handles{}
		{
			static_assert(Traits::IsCopyConstructable, "T::T(const T&) has not been defined");
		}
		constexpr Allocator(Allocator&& other) noexcept
			: m_Allocator{ __MOVE(other.m_Allocator) }
			, m_TakenMemoryLocations{ __MOVE(other.m_TakenMemoryLocations) }
			, m_Handles{ __MOVE(other.m_Handles) }
		{
			static_assert(Traits::IsMoveConstructable, "T::T(T&&) has not been defined");

			for (IHandle* const pHandle : m_Handles)
				pHandle->m_pAllocator = &m_Allocator;
			
			other.m_TakenMemoryLocations.clear();
			other.m_Handles.clear();
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
			m_Handles = __MOVE(other.m_Handles);

			for (IHandle* const pHandle : m_Handles)
				pHandle->m_pAllocator = &m_Allocator;

			other.m_TakenMemoryLocations.clear();
			other.m_Handles.clear();

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

			Handle<U>* pHandle{ new Handle<U>{ &m_Allocator, memLoc } };
			pHandle->m_IsValid = true;
			m_Handles.push_back(pHandle);

			return *pHandle;
		}

		template<typename U>
		constexpr void Deallocate(Handle<U>& handle)
		{
			static_assert(Traits::CanDeallocate, "void T::Deallocate() has not been defined");
			static_assert(Traits::template CanDestroy<U>, "void T::Destroy() has not been defined");

			m_Allocator.Destroy(&handle);
			m_Allocator.Deallocate(&handle);

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

			handle.m_IsValid = false;

			delete (*it);
			m_Handles.erase(it);
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

		__NODISCARD constexpr IHandle* const GetHandle(const uint64_t memLoc)
		{
			for (IHandle* const pHandle : m_Handles)
				if (pHandle->m_MemoryLocation == memLoc)
					return pHandle;

			return nullptr;
		}
		__NODISCARD constexpr IHandle* const GetHandle(const uint64_t memLoc) const
		{
			for (const IHandle* const pHandle : m_Handles)
				if (pHandle->m_MemoryLocation == memLoc)
					return pHandle;

			return nullptr;
		}

	private:
		T m_Allocator;
		std::vector<uint64_t> m_TakenMemoryLocations;
		std::vector<IHandle*> m_Handles;
	};
}