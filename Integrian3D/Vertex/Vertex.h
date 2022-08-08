#pragma once

#include "../EngineConstants.h"

namespace Integrian3D
{
	struct Vertex final
	{
		/* [TODO]: Change this type to a Vector3 and a RGBColour */
		explicit Vertex(const float x, const float y, const float z, const float r, const float g, const float b, const float a = 1.f)
			: Position{ x,y,z }
			, R{ r }
			, G{ g }
			, B{ b }
			, A{ a }
		{}

		Vector3f Position;
		float R, G, B, A;
	};
}