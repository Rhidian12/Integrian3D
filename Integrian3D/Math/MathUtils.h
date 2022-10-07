#pragma once

#include "../EngineConstants.h"

#include <type_traits> /* std::enable_if */
#include <limits> /* std::numeric_limits */
#include <random> /* std::random_device */
#include <glm.hpp> /* GLM */

namespace Integrian3D
{
	namespace MathUtils
	{
		inline constexpr double Pi{ 3.14159265358979323846264338327950288 };
		inline constexpr double Pi_2{ Pi / 2.0 };
		inline constexpr double Pi_4{ Pi / 4.0 };
		inline uint32_t Seed{}; /* Set in Core::CreateCore() */

		inline constexpr glm::vec3 Up{ 0.f, 1.f, 0.f };
		inline constexpr glm::vec3 Right{ 1.f, 0.f, 0.f };
		inline constexpr glm::vec3 Forward{ 0.f, 0.f, 1.f };

		using Vec2D = glm::vec<2, double>;
		/* [TODO]: Add other type defines! */

		namespace
		{
			std::mt19937 mt{ Seed };
		}

		/* magic lol https://www.codeproject.com/Articles/69941/Best-Square-Root-Method-Algorithm-Function-Precisi */
		/* works only for floats since it depends on IEEE 754 single precision floating point format */
		/* [TODO]: All __INLINE functions should be profiled to check if it's better to inline them or not */
		__INLINE constexpr float Sqrtf(const float n)
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
			return static_cast<T>(deg * Pi / static_cast<T>(180.f));
		}

		template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
		constexpr T ToDegrees(const T rad)
		{
			return static_cast<T>(rad * static_cast<T>(180.f) / Pi);
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

		__INLINE float RandomF(const float min, const float max)
		{
			std::uniform_real_distribution<float> dist(min, max);

			return dist(mt);
		}

		__INLINE constexpr glm::vec3 RandomVec3(const float min, const float max)
		{
			return glm::vec3{ RandomF(min, max),RandomF(min, max),RandomF(min, max) };
		}
	}
}