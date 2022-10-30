#pragma once

#include "../EngineConstants.h"
#include "../Math/Math.h"

#include <string_view> /* std::string_view */
#include <type_traits> /* std::true_type, ... */

namespace Integrian3D
{
	namespace Utils
	{
		/* Reference for TypeName: https://stackoverflow.com/questions/35941045/can-i-obtain-c-type-names-in-a-constexpr-way */

		template<typename T>
		constexpr std::string_view ConstexprTypeName();

		template<>
		constexpr std::string_view ConstexprTypeName<void>() { return "void"; }

		/* Should have internal linkage and therefore be unavailable to other files */
		namespace
		{
			template<typename T>
			constexpr std::string_view WrappedTypeName()
			{
				return __FUNCSIG__;
			}
		}

		template <typename T>
		constexpr std::string_view ConstexprTypeName()
		{
			constexpr std::string_view wrappedName(WrappedTypeName<T>());

			constexpr size_t endOfType{ wrappedName.find_last_of('>') };
			constexpr size_t beginOfType{ Math::Max(wrappedName.find_last_of(' '), wrappedName.find_last_of('<')) };

			return wrappedName.substr(beginOfType + 1, endOfType - beginOfType - 1);
		}

		__INLINE constexpr uint32_t ConstexprStringHash(const char* pKey, size_t count)
		{
			uint32_t p = 31;
			const uint32_t m = static_cast<uint32_t>(1e9) + 9;
			uint32_t hash_value = 0;
			uint32_t p_pow = 1;
			const char* c = pKey;
			while (count > 0)
			{
				hash_value = (hash_value + (*c - static_cast<uint32_t>('a') + 1) * p_pow) % m;
				p_pow = (p_pow * p) % m;
				c++;
				--count;
			}

			return hash_value;
		}

		/* https://stackoverflow.com/questions/600293/how-to-check-if-a-number-is-a-power-of-2 */
		__INLINE constexpr bool IsPowerOfTwo(const uint64_t val)
		{
			return (val & (val - 1)) == 0;
		}

		/* https://indiegamedev.net/2022/03/27/custom-c20-memory-allocators-for-stl-containers/ */
		__INLINE constexpr uint64_t AlignForward(const void* const p, const uint64_t alignment)
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
		constexpr uint64_t AlignForward(const void* const p, const uint64_t alignment)
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