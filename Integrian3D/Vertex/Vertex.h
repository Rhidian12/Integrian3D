#pragma once

#include "../EngineConstants.h"

namespace Integrian3D
{
	struct Vertex final
	{
		Vertex() = default;
		explicit Vertex(const Vector3f& position)
			: Position{ position }
			, Colour{}
		{}
		Vertex(const Vector3f& position, const RGBColour& colour)
			: Position{ position }
			, Colour{ colour }
		{}

		Vector3f Position;
		RGBColour Colour;
	};
}