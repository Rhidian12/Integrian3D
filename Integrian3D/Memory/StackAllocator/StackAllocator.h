#pragma once

#include "../../EngineConstants.h"
#include "../MemoryUtils.h"

namespace Integrian3D::Memory
{
	template<uint64_t N>
	class StackAllocator final
	{
	public:
		constexpr StackAllocator()
			: Buffer{}
			, StackPointer{}
		{}

#pragma region Rule of 5
		StackAllocator(const StackAllocator&) noexcept = delete;
		StackAllocator(StackAllocator&&) noexcept = delete;

		StackAllocator& operator=(const StackAllocator&) noexcept = delete;
		StackAllocator& operator=(StackAllocator&&) noexcept = delete;
#pragma endregion

		template<typename T>
		__NODISCARD constexpr T* Allocate(const uint64_t nrOfElements, const uint64_t align = alignof(T))
		{
			__ASSERT(nrOfElements != 0 && "StackAllocator::Allocate() > Cannot allocate 0 elements");

			const uint64_t memRequirement{ nrOfElements * sizeof(T) };

			if (memRequirement <= N - StackPointer)
			{
				T* data = reinterpret_cast<T*>(&Buffer[StackPointer]);
				
				const uint64_t alignment{ AlignForward(data, align) };
				data += alignment;

				StackPointer += memRequirement + alignment;

				__ASSERT(StackPointer <= N && "StackAllocator::Allocate() > Ran out of memory");

				return data;
			}

			__CASSERT(false && "StackAllocator::Allocate() > Ran out of memory");

			return nullptr;
		}

		template<typename T>
		constexpr void Deallocate(T* p, uint64_t n)
		{
			if (IsPointerInBuffer(p))
			{
				StackPointer -= n * sizeof(T);
			}
		}

		__NODISCARD constexpr uint64_t Capacity() const
		{
			return N;
		}

		__NODISCARD constexpr uint64_t Size() const
		{
			return StackPointer;
		}

		__NODISCARD constexpr uint64_t MaxSize() const
		{
			return N;
		}

		__NODISCARD constexpr const char* Data() const
		{
			return Buffer;
		}

		__NODISCARD constexpr bool operator==(const StackAllocator& other) const
		{
			return Buffer == other.Buffer && Size() == other.Size();
		}

		__NODISCARD constexpr bool operator!=(const StackAllocator& other) const
		{
			return !(*this == other);
		}

	private:
		template<typename T>
		constexpr bool IsPointerInBuffer(const T* p) const
		{
			return std::greater_equal<const T*>{}(p, reinterpret_cast<const T*>(&Buffer)) && std::less<const T*>{}(p, reinterpret_cast<const T*>(&Buffer + N));
		}

		char Buffer[N];
		uint64_t StackPointer;
	};
}