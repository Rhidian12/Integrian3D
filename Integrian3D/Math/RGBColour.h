#pragma once

#include "../Math/MathUtils.h"

namespace Integrian3D
{
	struct RGBColour final
	{
		constexpr RGBColour() = default;
		constexpr RGBColour(const float r, const float g, const float b, const float a = 1.f)
			: R{ r }
			, G{ g }
			, B{ b }
			, A{ a }
		{}

		constexpr RGBColour& NormalizeToOne()
		{
			R /= 255.f;
			B /= 255.f;
			G /= 255.f;

			MathUtils::Clamp(A, 0.f, 1.f);

			return *this;
		}

		constexpr RGBColour GetNormalized() const
		{
			RGBColour temp{ R,G,B,A };
			temp.NormalizeToOne();
			return temp;
		}

		float R, G, B, A;
	};
}