#pragma once

#include <math.h> /* sqrtf() */

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

		T Magnitude() const
		{
			return static_cast<T>(sqrtf(MagnitudeSquared()));
		}

		T MagnitudeSquared() const
		{
			return Dot(*this);
		}

		T X, Y;
	};
}