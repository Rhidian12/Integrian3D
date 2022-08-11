#pragma once

namespace Integrian3D
{
	namespace MathUtils
	{
		/* magic lol https://www.codeproject.com/Articles/69941/Best-Square-Root-Method-Algorithm-Function-Precisi */
		inline constexpr float Sqrtf(const float n)
		{
			union
			{
				int i;
				float x;
			} u;

			u.x = n;
			u.i = (1 << 29) + (u.i >> 1) - (1 << 22);
			return u.x;
		}

		inline constexpr double Sqrtf(const double n)
		{
			union
			{
				int i;
				double x;
			} u;

			u.x = n;
			u.i = (1 << 29) + (u.i >> 1) - (1 << 22);
			return u.x;
		}
	}
}