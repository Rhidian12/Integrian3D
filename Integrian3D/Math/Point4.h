#pragma once

namespace Integrian3D
{
	template<typename T>
	struct Point4 final
	{
		Point4() = default;
		Point4(const T x, const T y, const T z, const T w)
			: X{ x }
			, Y{ y }
			, Z{ z }
			, W{ w }
		{}

		T X, Y, Z, W;
	};
}