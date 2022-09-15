#pragma once

#include "../EngineConstants.h"
#include "../Math/RGBColour.h"

#include <glm.hpp> /* GLM */

namespace Integrian3D
{
	struct Vertex final
	{
		Vertex() = default;
		explicit Vertex(const glm::vec3& position)
			: Position{ position }
			, Colour{}
			, UV{}
		{}
		Vertex(const glm::vec3& position, const RGBColour& colour)
			: Position{ position }
			, Colour{ colour }
			, UV{}
		{}
		Vertex(const glm::vec3& position, const RGBColour& colour, const glm::vec2& uv)
			: Position{ position }
			, Colour{ colour }
			, UV{ uv }
		{}

		glm::vec3 Position;
		RGBColour Colour;
		glm::vec2 UV;
	};
}