#include "CameraComponent.h"

#include "../TransformComponent/TransformComponent.h"
#include "../../SceneManager/SceneManager.h"
#include "../../Math/Math.h"

#include <gtc/matrix_transform.hpp>

namespace Integrian3D
{
	CameraComponent::CameraComponent(const float nearPlane, const float farPlane, const float fov, const float aspectRatio, const double speed)
		: NearPlane{ nearPlane }
		, FarPlane{ farPlane }
		, FOV{ fov }
		, AspectRatio{ aspectRatio }
		, Speed{ speed }
		, View{ 1.f }
		, Projection{ 1.f }
	{
		const Math::Vec3D startPos{ 0.0, 0.0, -3.0 };
		View = glm::lookAt(startPos, startPos + Math::Forward, Math::Up);
		Projection = glm::perspective(FOV, AspectRatio, NearPlane, FarPlane);
	}

	void CameraComponent::SetView(const glm::mat4& view)
	{
		View = view;
	}

	void CameraComponent::SetSpeed(const float speed)
	{
		Speed = speed;
	}
}