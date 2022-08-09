#pragma once

namespace Integrian3D
{
	template<typename T>
	struct Point2 final
	{
		Point2() = default;
		Point2(const T x, const T y)
			: X{ x }
			, Y{ y }
		{}

		T X, Y;
	};
}