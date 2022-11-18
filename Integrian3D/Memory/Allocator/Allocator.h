#pragma once

#include "../../EngineConstants.h"
#include "../AllocatorTraits/AllocatorTraits.h"

namespace Integrian3D::Memory
{
	template<typename T, typename Alloc>
	class Handle final
	{
	public:
		explicit constexpr Handle(Alloc& alloc, const uint64_t memLoc)
			: m_Allocator{ alloc }
			, m_MemoryLocation{ memLoc }
		{}

		constexpr Handle(const Handle&) noexcept = delete;
		constexpr Handle(Handle&&) noexcept = delete;
		constexpr Handle& operator=(const Handle&) noexcept = delete;
		constexpr Handle& operator=(Handle&&) noexcept = delete;

		__NODISCARD __INLINE constexpr T* operator->()
		{
			return reinterpret_cast<T*>(static_cast<char*>(m_Allocator.Data()) + m_MemoryLocation * sizeof(T));
		}
		__NODISCARD __INLINE constexpr const T* operator->() const
		{
			return reinterpret_cast<T*>(static_cast<char*>(m_Allocator.Data()) + m_MemoryLocation * sizeof(T));
		}

		__NODISCARD __INLINE constexpr T& operator*()
		{
			return *reinterpret_cast<T*>(static_cast<char*>(m_Allocator.Data()) + m_MemoryLocation * sizeof(T));
		}
		__NODISCARD __INLINE constexpr const T& operator*() const
		{
			return *reinterpret_cast<T*>(static_cast<char*>(m_Allocator.Data()) + m_MemoryLocation * sizeof(T));
		}

	private:
		Alloc& m_Allocator;
		uint64_t m_MemoryLocation;
	};

	template<typename T>
	class Allocator
	{
		using Traits = AllocatorTraits<T>;
	public:
		constexpr explicit Allocator(const T& alloc = T{})
			: m_Allocator{ alloc }
			, m_Key{}
		{}

#pragma region RuleOf5
		constexpr Allocator(const Allocator& other) noexcept
			: m_Allocator{ other.m_Allocator }
			, m_Key{}
		{
			static_assert(Traits::IsCopyConstructable, "T::T(const T&) has not been defined");
		}
		constexpr Allocator(Allocator&& other) noexcept
			: m_Allocator{ __MOVE(T, other.m_Allocator) }
			, m_Key{ __MOVE(uint64_t, other.m_Key) }
		{
			static_assert(Traits::IsMoveConstructable, "T::T(T&&) has not been defined");
		}
		constexpr Allocator& operator=(const Allocator& other) noexcept
		{
			static_assert(Traits::IsCopyAssignable, "T& T::operator=(const T&) has not been defined");

			m_Allocator = other.m_Allocator;
			m_Key = 0;

			return *this;
		}
		constexpr Allocator& operator=(Allocator&& other) noexcept
		{
			static_assert(Traits::IsMoveAssignable, "T& T::operator=(T&&) has not been defined");

			m_Allocator = __MOVE(T, other.m_Allocator);
			m_Key = __MOVE(uint64_t, other.m_Key);

			return *this;
		}
#pragma endregion

		template<typename U>
		__NODISCARD constexpr Handle<U, T> Allocate(const uint64_t nrOfElements, const uint64_t align = alignof(U))
		{
			static_assert(Traits::template CanAllocate<U>, "T::Allocate() has not been defined");
			static_assert(Traits::HasData, "T::Data() has not been defined");

#pragma warning ( push )
#pragma warning ( disable : 4834 ) // warning C4834: discarding return value of function with 'nodiscard' attribute
			m_Allocator.Allocate<U>(nrOfElements, align);
#pragma warning ( pop )

			return Handle<U, T>{ m_Allocator, m_Key++ * nrOfElements };
		}

		template<typename U>
		constexpr void Deallocate(U* const pData)
		{
			static_assert(Traits::template CanDeallocate<U>, "T::Deallocate() has not been defined");

			m_Allocator.Deallocate(pData);

			--m_Key;
		}

	private:
		T m_Allocator;
		uint64_t m_Key;
	};
}