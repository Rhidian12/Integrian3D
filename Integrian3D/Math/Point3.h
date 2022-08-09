#pragma once

namespace Integrian3D
{
	template<typename T>
	struct Point3 final
	{
		Point3() = default;
		Point3(const T x, const T y, const T z)
			: X{ x }
			, Y{ y }
			, Z{ z }
		{}

		T X, Y, Z;
	};
}