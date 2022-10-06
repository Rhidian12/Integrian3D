#include "CameraComponent.h"

#include "../TransformComponent/TransformComponent.h"
#include "../../SceneManager/SceneManager.h"
#include "../../Math/MathUtils.h"

#include <gtc/matrix_transform.hpp>

namespace Integrian3D
{
	CameraComponent::CameraComponent(const float nearPlane, const float farPlane, const float fov, const float aspectRatio, const float speed)
		: NearPlane{ nearPlane }
		, FarPlane{ farPlane }
		, FOV{ fov }
		, AspectRatio{ aspectRatio }
		, Speed{ speed }
		, View{ 1.f }
		, Projection{ 1.f }
	{
		const glm::vec3 startPos{ 0.f, 0.f, -3.f };
		View = glm::lookAt(startPos, startPos + MathUtils::Forward, MathUtils::Up);
		Projection = glm::perspective(FOV, AspectRatio, NearPlane, FarPlane);
	}

	void CameraComponent::SetView(const glm::vec3& trans)
	{
		View = glm::lookAt(trans, trans + MathUtils::Forward, MathUtils::Up);
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