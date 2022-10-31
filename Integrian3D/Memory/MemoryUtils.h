#pragma once

#include "../EngineConstants.h"

namespace Integrian3D
{
	namespace Memory
	{
		/* https://stackoverflow.com/questions/600293/how-to-check-if-a-number-is-a-power-of-2 */
		__INLINE __NODISCARD constexpr bool IsPowerOfTwo(const uint64_t val)
		{
			return (val & (val - 1)) == 0;
		}

		/* https://indiegamedev.net/2022/03/27/custom-c20-memory-allocators-for-stl-containers/ */
		__INLINE __NODISCARD uint64_t AlignForward(const void* const p, const uint64_t alignment)
		{
			// Example: Suppose p = 12 == 0xC and alignment = 8 == 0x8
			// (12 + (8 - 1)) & ~(8 - 1)
			// 19 & ~7
			// 0x0013 & ~0x0007
			// 0x0013 & 0xFFF8
			// 0x0010
			// 16

			return (reinterpret_cast<uint64_t>(p) + (alignment - 1)) & ~(alignment - 1) - reinterpret_cast<uint64_t>(p);
		}

		template<typename ... Ts>
		__NODISCARD uint64_t AlignForward(const void* const p, const uint64_t alignment)
		{
			uint64_t adjustment = AlignForward(p, alignment);

			uint64_t neededSpace = (sizeof(Ts...) + ...);

			if (adjustment < neededSpace)
			{
				neededSpace -= adjustment;

				adjustment += alignment * (neededSpace / alignment);

				if (neededSpace % alignment > 0)
				{
					adjustment += alignment;
				}
			}

			return adjustment;
		}
	}
}