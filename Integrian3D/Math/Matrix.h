#pragma once

#include "Point4.h"

namespace Integrian3D
{
	template<int R, int C, typename T>
	struct Matrix final
	{
		Matrix() = default;

		T operator()(const int r, const int c) const
		{
			return Data[r][c];
		}
		T& operator()(const int r, const int c)
		{
			return Data[r][c];
		}

		T Data[R][C];
	};
}