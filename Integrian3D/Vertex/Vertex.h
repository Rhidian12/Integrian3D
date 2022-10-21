#pragma once

#include "../EngineConstants.h"
#include "../Math/RGBColour.h"

#include "../Math/Math.h"

namespace Integrian3D
{
	struct Vertex final
	{
		Vertex() = default;
		explicit Vertex(const Math::Vec3D& position)
			: Position{ position }
			, UV{}
		{}
		Vertex(const Math::Vec3D& position, const Math::Vec2D& uv)
			: Position{ position }
			, UV{ uv }
		{}

		Math::Vec3D Position;
		Math::Vec2D UV;
	};
}