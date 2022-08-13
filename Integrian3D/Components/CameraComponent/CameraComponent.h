#pragma once

#include <glm.hpp>

namespace Integrian3D
{
	class CameraComponent final
	{
	public:
		CameraComponent(const float nearPlane, const float farPlane, const float fov);

	private:
		float NearPlane;
		float FarPlane;
		float FOV;

		glm::mat4 View;
		glm::mat4 Projection;
	};
}