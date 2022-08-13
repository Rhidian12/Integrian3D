#include "CameraComponent.h"

namespace Integrian3D
{
	CameraComponent::CameraComponent(const float nearPlane, const float farPlane, const float fov)
		: NearPlane{ nearPlane }
		, FarPlane{ farPlane }
		, FOV{ fov }
	{
	}
}