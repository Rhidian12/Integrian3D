#pragma once

#include <math.h> /* sqrtf */

namespace Integrian3D
{
	template<typename T>
	struct Vector3 final
	{
		explicit Vector3()
			: X{}
			, Y{}
			, Z{}
		{}
		explicit Vector3(const T x, const T y, const T z)
			: X{ x }
			, Y{ y }
			, Z{ z }
		{}

		T X, Y, Z;

		T Dot(const Vector3<T>& vec) const
		{
			return X * vec.X + Y * vec.Y + Z * vec.Z;
		}

		T Magnitude() const
		{
			return static_cast<T>(sqrtf(MagnitudeSquared()));
		}

		T MagnitudeSquared() const
		{
			return Dot(*this);
		}

		Vector3 GetNormalized() const
		{

		}
	};
}