#pragma once

#include "../EngineConstants.h"
#include "../Math/MathUtils.h"

#include <gtc/matrix_transform.hpp>

namespace Integrian3D
{
	namespace Systems
	{
		void TranslateCamera(const glm::vec3& dir);
		void RotateCamera(const MathUtils::Vec2D& mousePos, const MathUtils::Vec2D& lastMousePos);
	}
}