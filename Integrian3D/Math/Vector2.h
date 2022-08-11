#pragma once

#include "../Utils/Utils.h"
#include "MathUtils.h"

#include <math.h> /* sqrtf() */
#include <assert.h> /* assert() */

namespace Integrian3D
{
	template<typename T>
	struct Vector2 final
	{
		constexpr Vector2() = default;
		constexpr Vector2(const T x, const T y)
			: X{ x }
			, Y{ y }
		{}

		constexpr T Dot(const Vector2& vec) const
		{
			return X * vec.X + Y * vec.Y;
		}

		constexpr T Cross(const Vector2& vec) const
		{
			return X * vec.Y - Y * vec.X;
		}

		constexpr T Magnitude() const
		{
			return static_cast<T>(MathUtils::Sqrtf(MagnitudeSquared()));
		}

		constexpr T MagnitudeSquared() const
		{
			return Dot(*this);
		}

		constexpr Vector2& Normalize()
		{
			const T magnitude{ Magnitude() };

			assert(!Utils::AreEqual(magnitude, static_cast<T>(0.f)) && "Vector2::Normalize() > Magnitude may not be 0");

			X /= magnitude;
			Y /= magnitude;

			return *this;
		}

		constexpr Vector2 GetNormalized() const
		{
			Vector2 temp{ X, Y };
			temp.Normalize();
			return temp;
		}

		T X, Y;
	};
}