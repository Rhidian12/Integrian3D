#pragma once

#include "../../EngineConstants.h"

#include <glm.hpp> /* GLM */

namespace Integrian3D
{
	class CameraComponent final
	{
	public:
		CameraComponent(const float nearPlane, const float farPlane, const float fov,
			const float aspectRatio, const double speed = 10.0);

		void SetView(const glm::mat4& view);

		void SetSpeed(const float speed);
		__NODISCARD double GetSpeed() const { return Speed; }

		__NODISCARD const glm::mat4& GetView() const { return View; }
		__NODISCARD const glm::mat4& GetProjection() const { return Projection; }

	private:
		float NearPlane;
		float FarPlane;
		float FOV;
		float AspectRatio;
		double Speed;

		glm::mat4 View;
		glm::mat4 Projection;
	};
}