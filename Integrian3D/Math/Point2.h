#pragma once

namespace Integrian3D
{
	template<typename T>
	struct Point2 final
	{
		constexpr Point2() = default;
		constexpr Point2(const T x, const T y)
			: X{ x }
			, Y{ y }
		{}

		T X, Y;
	};
}