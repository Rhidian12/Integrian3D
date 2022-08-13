#include "CameraComponent.h"

#include "../TransformComponent/TransformComponent.h"
#include "../../SceneManager/SceneManager.h"

#include <gtc/matrix_transform.hpp>

namespace Integrian3D
{
	CameraComponent::CameraComponent(const float nearPlane, const float farPlane, const float fov, const float aspectRatio)
		: NearPlane{ nearPlane }
		, FarPlane{ farPlane }
		, FOV{ fov }
		, AspectRatio{ aspectRatio }
		, View{ 1.f }
		, Projection{ 1.f }
	{
		View = glm::translate(View, glm::vec3{ 0.f, 0.f, -10.f });
		Projection = glm::perspective(FOV, AspectRatio, NearPlane, FarPlane);
	}
}