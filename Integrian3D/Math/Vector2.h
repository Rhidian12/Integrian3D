#pragma once

namespace Integrian3D
{
	template<typename T>
	struct Vector2 final
	{
		Vector2() = default;
		Vector2(const T x, const T y)
			: X{ x }
			, Y{ y }
		{}

		T X, Y;
	};
}