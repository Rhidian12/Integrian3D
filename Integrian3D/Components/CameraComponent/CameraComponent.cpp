#include "CameraComponent.h"

#include "../TransformComponent/TransformComponent.h"
#include "../../SceneManager/SceneManager.h"

#include <gtc/matrix_transform.hpp>

namespace Integrian3D
{
	CameraComponent::CameraComponent(const float nearPlane, const float farPlane, const float fov)
		: NearPlane{ nearPlane }
		, FarPlane{ farPlane }
		, FOV{ fov }
		, View{ 1.f }
		, Projection{ 1.f }
	{}
}