#pragma once

#include "../Utils/Utils.h"

#include <math.h> /* sqrtf() */
#include <assert.h> /* assert() */

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

		T Dot(const Vector3& vec) const
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

		Vector3& Normalize()
		{
			const T magnitude{ Magnitude() };

			assert(!Utils::AreEqual(magnitude, static_cast<T>(0.f)) && "Vector3::Normalize() > Magnitude may not be 0");

			X /= magnitude;
			Y /= magnitude;
			Z /= magnitude;

			return *this;
		}

		Vector3 GetNormalized() const
		{
			Vector3<T> temp{ X, Y, Z };
			temp.Normalize();
			return temp;
		}
	};
}