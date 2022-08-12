#pragma once

#include <type_traits> /* std::enable_if */
#include <limits> /* std::numeric_limits */

namespace Integrian3D
{
	namespace MathUtils
	{
		constexpr double Pi{ 3.14159265358979323846264338327950288 };
		constexpr double Pi_2{ Pi / 2.0 };
		constexpr double Pi_4{ Pi / 4.0 };

		/* magic lol https://www.codeproject.com/Articles/69941/Best-Square-Root-Method-Algorithm-Function-Precisi */
		/* works only for floats since it depends on IEEE 754 single precision floating point format */
		__forceinline constexpr float Sqrtf(const float n)
		{
			union
			{
				int i;
				float x;
			} u;

			u.x = n;
			u.i = (1 << 29) + (u.i >> 1) - (1 << 22) - 0x4B0D2;
			return u.x;
		}

		template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
		constexpr T ToRadians(const T deg)
		{
			return deg * Pi / static_cast<T>(180.f);
		}

		template<typename T, typename = std::enable_if_t<std::is_fundamental_v<T>>>
		constexpr bool AreEqual(const T a, const T b)
		{
			return static_cast<T>(fabs(a - b)) <= std::numeric_limits<T>::epsilon();
		}

		template<typename T>
		constexpr void Clamp(T& value, const T& min, const T& max)
		{
			if (value < min)
			{
				value = min;
			}

			if (value > max)
			{
				value = max;
			}
		}
	}
}