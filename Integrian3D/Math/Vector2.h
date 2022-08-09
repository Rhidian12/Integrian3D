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

		T Dot(const Vector2& vec) const
		{
			return X * vec.X + Y * vec.Y;
		}

		T Cross(const Vector2& vec) const
		{
			return X * vec.Y - Y * vec.X;
		}

		T X, Y;
	};
}