#pragma once

#include "../Utils/Utils.h"
#include "MathUtils.h"

#include <math.h> /* sqrtf() */
#include <assert.h> /* assert() */

namespace Integrian3D
{
	template<typename T>
	struct Vector3 final
	{
		constexpr Vector3() = default;
		constexpr Vector3(const T x, const T y, const T z)
			: X{ x }
			, Y{ y }
			, Z{ z }
		{}

		constexpr T Dot(const Vector3& vec) const
		{
			return X * vec.X + Y * vec.Y + Z * vec.Z;
		}

		constexpr Vector3 Cross(const Vector3& vec) const
		{
			return Vector3
			{
				Y * vec.Z - Z * vec.Y,
				X * vec.Y - Y * vec.X,
				Z * vec.X - X * vec.Z
			};
		}

		constexpr T Magnitude() const
		{
			return static_cast<T>(MathUtils::Sqrtf(MagnitudeSquared()));
		}

		constexpr T MagnitudeSquared() const
		{
			return Dot(*this);
		}

		constexpr Vector3& Normalize()
		{
			const T magnitude{ Magnitude() };

			assert(!Utils::AreEqual(magnitude, static_cast<T>(0.f)) && "Vector3::Normalize() > Magnitude may not be 0");

			X /= magnitude;
			Y /= magnitude;
			Z /= magnitude;

			return *this;
		}

		constexpr Vector3 GetNormalized() const
		{
			Vector3<T> temp{ X, Y, Z };
			temp.Normalize();
			return temp;
		}

		T X, Y, Z;
	};
}