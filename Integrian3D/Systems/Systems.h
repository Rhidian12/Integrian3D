#pragma once

#include "../EngineConstants.h"

#include <gtc/matrix_transform.hpp>

namespace Integrian3D
{
	namespace Systems
	{
		void TransformCamera(const glm::vec3& dir, const glm::vec3& rot);
	}
}