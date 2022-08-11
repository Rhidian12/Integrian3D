#pragma once

namespace Integrian3D
{
	template<typename T>
	struct Point3 final
	{
		constexpr Point3() = default;
		constexpr Point3(const T x, const T y, const T z)
			: X{ x }
			, Y{ y }
			, Z{ z }
		{}

		T X, Y, Z;
	};
}