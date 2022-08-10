#pragma once

#include "../EngineConstants.h"

namespace Integrian3D
{
#ifdef __VERTEX_COLOUR__
	struct Vertex final
	{
		Vertex() = default;
		explicit Vertex(const Vector3f& position)
			: Position{ position }
			, Colour{}
			, UV{}
		{}
		Vertex(const Vector3f& position, const RGBColour& colour)
			: Position{ position }
			, Colour{ colour }
			, UV{}
		{}
		Vertex(const Vector3f& position, const RGBColour& colour, const Point2f& uv)
			: Position{ position }
			, Colour{ colour }
			, UV{ uv }
		{}

		Vector3f Position;
		RGBColour Colour;
		Point2f UV;
	};
#else
	struct Vertex final
	{
		Vertex() = default;
		explicit Vertex(const Vector3f& position)
			: Position{ position }
			, UV{}
		{}
		Vertex(const Vector3f& position, const Point2f& uv)
			: Position{ position }
			, UV{ uv }
		{}

		Vector3f Position;
		Point2f UV;
	};
#endif
}