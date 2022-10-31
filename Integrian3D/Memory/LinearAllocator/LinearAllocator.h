#pragma once

#include "../../EngineConstants.h"
#include "../MemoryUtils.h"

namespace Integrian3D::Memory
{
	class LinearAllocator final
	{
	public:
		using CanMove = std::true_type;
		using CanCopy = std::true_type;

		LinearAllocator();
		explicit LinearAllocator(const uint64_t nrOfBytes);
		~LinearAllocator();

#pragma region Rule of 5
		LinearAllocator(const LinearAllocator& other) noexcept
			: Start{}
			, End{}
			, Current{}
		{
			Start = malloc(other.Capacity());
			End = static_cast<char*>(Start) + other.Capacity();
			Current = static_cast<char*>(Start);
		}
		LinearAllocator(LinearAllocator&& other) noexcept
			: Start{ __MOVE(void*, other.Start) }
			, End{ __MOVE(void*, other.End) }
			, Current{ __MOVE(void*, other.Current) }
		{}

		LinearAllocator& operator=(const LinearAllocator& other) noexcept
		{
			if (Start)
			{
				free(Start);
			}

			Start = malloc(other.Capacity());
			End = static_cast<char*>(Start) + other.Capacity();
			Current = static_cast<char*>(Start);

			return *this;
		}
		LinearAllocator& operator=(LinearAllocator&& other) noexcept
		{
			if (Start)
			{
				free(Start);
			}

			Start = __MOVE(void*, other.Start);
			End = __MOVE(void*, other.End);
			Current = __MOVE(void*, other.Current);

			return *this;
		}
#pragma endregion

		template<typename T>
		__NODISCARD constexpr T* Allocate(const uint64_t nrOfElements, const uint64_t align = alignof(T))
		{
			__ASSERT(nrOfElements != 0 && "LinearAllocator::Allocate() > Cannot allocate 0 elements");

			__ASSERT(static_cast<T*>(Current) < End && "LinearAllocator::Allocate() > Ran out of memory");

			T* data{ static_cast<T*>(Current) };

			const uint64_t alignment{ AlignForward(data, align) };
			data += alignment;

			Current = static_cast<char*>(Current) + nrOfElements * sizeof(T) + alignment;

			__ASSERT(static_cast<T*>(Current) <= End && "LinearAllocator::Allocate() > Ran out of memory");

			return static_cast<T*>(data);
		}

		__NODISCARD constexpr uint64_t Capacity() const { return static_cast<char*>(End) - static_cast<char*>(Start); }
		__NODISCARD constexpr uint64_t Size() const { return static_cast<char*>(Current) - static_cast<char*>(Start); }
		__NODISCARD constexpr uint64_t MaxSize() const { return Capacity(); }
		__NODISCARD constexpr const void* Data() const { return Start; }

		__NODISCARD constexpr bool operator==(const LinearAllocator& other) const
		{
			return Start == other.Start;
		}
		__NODISCARD constexpr bool operator!=(const LinearAllocator& other) const
		{
			return Start != other.Start;
		}

	private:
		void* Start;
		void* Current;
		void* End; /* points past allocated memory */
	};
}