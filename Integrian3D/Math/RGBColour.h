#pragma once

#include "../Utils/Utils.h"

namespace Integrian3D
{
	struct RGBColour final
	{
		RGBColour()
			: R{}
			, G{}
			, B{}
			, A{}
		{}
		RGBColour(const float r, const float g, const float b, const float a)
			: R{ r }
			, G{ g }
			, B{ b }
			, A{ a }
		{}

		RGBColour& NormalizeToOne()
		{
			R /= 255.f;
			B /= 255.f;
			G /= 255.f;

			Utils::Clamp(A, 0.f, 1.f);

			return *this;
		}

		RGBColour GetNormalized()
		{
			RGBColour temp{ R,G,B,A };
			temp.NormalizeToOne();
			return temp;
		}

		float R, G, B, A;
	};
}