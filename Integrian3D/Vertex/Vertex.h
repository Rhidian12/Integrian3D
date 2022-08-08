#pragma once

#include "../EngineConstants.h"

namespace Integrian3D
{
	struct Vertex final
	{
		/* [TODO]: Change this type to a Vector3 and a RGBColour */
		explicit Vertex()
			: Position{}
			, Colour{}
		{}
		explicit Vertex(const Vector3f& position)
			: Position{ position }
			, Colour{}
		{}
		explicit Vertex(const Vector3f& position, const RGBColour& colour)
			: Position{ position }
			, Colour{ colour }
		{}
		explicit Vertex(const float x, const float y, const float z, const float r, const float g, const float b, const float a = 1.f)
			: Position{ x,y,z }
			, Colour{ r,g,b,a }
		{}

		Vector3f Position;
		RGBColour Colour;
	};
}