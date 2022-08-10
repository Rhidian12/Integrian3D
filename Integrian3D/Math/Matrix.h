#pragma once

namespace Integrian3D
{
	template<int R, int C, typename T>
	struct Matrix final
	{
		T Data[R][C];
	};
}