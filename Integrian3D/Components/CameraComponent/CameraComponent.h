#pragma once

#include "../../EngineConstants.h"

#include <glm.hpp> /* GLM */

namespace Integrian3D
{
	class CameraComponent final
	{
	public:
		CameraComponent(const float nearPlane, const float farPlane, const float fov, const float aspectRatio, const float speed = 10.f);

		void SetView(const glm::mat4& view);

		void SetSpeed(const float speed);
		__NODISCARD float GetSpeed() const { return Speed; }

		__NODISCARD const glm::mat4& GetView() const { return View; }
		__NODISCARD const glm::mat4& GetProjection() const { return Projection; }

	private:
		float NearPlane;
		float FarPlane;
		float FOV;
		float AspectRatio;
		float Speed;

		glm::mat4 View;
		glm::mat4 Projection;
	};
}