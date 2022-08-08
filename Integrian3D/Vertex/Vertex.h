#pragma once

namespace Integrian3D
{
	struct Vertex final
	{
		/* [TODO]: Change this type to a Vector3 and a RGBColour */
		explicit Vertex(const float x, const float y, const float z, const float r, const float g, const float b, const float a = 1.f)
			: X{ x }
			, Y{ y }
			, Z{ z }
			, R{ r }
			, G{ g }
			, B{ b }
			, A{ a }
		{}

		float X, Y, Z;
		float R, G, B, A;
	};
}