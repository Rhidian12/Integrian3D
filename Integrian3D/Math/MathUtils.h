#pragma once

namespace Integrian3D
{
	namespace MathUtils
	{
		float inline __declspec (naked) __fastcall Sqrtf(const float n)
		{
			/* magic lol https://www.codeproject.com/Articles/69941/Best-Square-Root-Method-Algorithm-Function-Precisi */
			_asm fld qword ptr[esp + 4]
			_asm fsqrt
			_asm ret 8
		}

		double inline __declspec (naked) __fastcall Sqrtf(const double n)
		{
			_asm fld qword ptr[esp + 4]
				_asm fsqrt
			_asm ret 8
		}
	}
}