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
		LinearAllocator(const LinearAllocator& other) noexcept;
		LinearAllocator(LinearAllocator&& other) noexcept;

		LinearAllocator& operator=(const LinearAllocator& other) noexcept;
		LinearAllocator& operator=(LinearAllocator&& other) noexcept;
#pragma endregion

		template<typename T>
		__NODISCARD constexpr T* Allocate(const uint64_t nrOfElements, const uint64_t align = alignof(T))
		{
			__ASSERT(nrOfElements != 0 && "LinearAllocator::Allocate() > Cannot allocate 0 elements");

			const uint64_t alignment{ AlignForward(Current, align) };

			if (static_cast<char*>(Current) + nrOfElements * sizeof(T) + alignment >= End)
			{
				Reallocate<T>();
			}

			T* data{ static_cast<T*>(Current) };

			data += alignment;

			Current = static_cast<char*>(Current) + nrOfElements * sizeof(T) + alignment;

			return static_cast<T*>(data);
		}

		template<typename T>
		constexpr void Deallocate(T*) {}

		__NODISCARD constexpr uint64_t Capacity() const { return static_cast<char*>(End) - static_cast<char*>(Start); }
		__NODISCARD constexpr uint64_t Size() const { return static_cast<char*>(Current) - static_cast<char*>(Start); }
		__NODISCARD constexpr uint64_t MaxSize() const { return std::numeric_limits<uint64_t>::max(); }
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
		template<typename T>
		constexpr void Reallocate()
		{
			const uint64_t size{ Size() };
			const uint64_t newCap{ CalculateNewCapacity(size + 1 + sizeof(T)) };

			void* pData = new char[newCap] {};

			memcpy(pData, Start, size);

			delete[] Start;

			Start = pData;
			End = static_cast<char*>(pData) + newCap;
			Current = static_cast<char*>(pData) + size;
		}

		__NODISCARD constexpr uint64_t CalculateNewCapacity(const uint64_t min) const
		{
			const uint64_t oldCap{ Capacity() };
			const uint64_t maxCap{ MaxSize() };

			if (oldCap > maxCap - oldCap / 2u)
			{
				return maxCap;
			}

			const uint64_t newCap{ oldCap + oldCap / 2u };

			// If our growth is insufficient, return just the bare minimum
			if (newCap < min)
			{
				return min;
			}

			return newCap;
		}

		void* Start;
		void* Current;
		void* End; /* points past allocated memory */
	};
}