#pragma once

namespace Integrian3D
{
	namespace MathUtils
	{
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
	}
}