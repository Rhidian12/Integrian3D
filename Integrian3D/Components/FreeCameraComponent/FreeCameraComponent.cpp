#include "FreeCameraComponent.h"

#include "../TransformComponent/TransformComponent.h"
#include "../../SceneManager/SceneManager.h"
#include "../../Math/Math.h"
#include "../../InputManager/InputManager.h"
#include "../../Timer/Timer.h"

#include <gtc/matrix_transform.hpp>

namespace Integrian3D
{
	FreeCameraComponent::FreeCameraComponent(const float nearPlane, const float farPlane, const float fov, const float aspectRatio, const double speed)
		: m_NearPlane{ nearPlane }
		, m_FarPlane{ farPlane }
		, m_FOV{ fov }
		, m_AspectRatio{ aspectRatio }
		, m_Speed{ speed }
		, m_View{ 1.f }
		, m_Projection{ 1.f }
	{
		const Math::Vec3D startPos{ 0.0, 0.0, -3.0 };
		m_View = glm::lookAt(startPos, startPos + Math::Forward, Math::Up);
		m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane);
	}

	void FreeCameraComponent::UpdateView(const TransformComponent& transform)
	{
		m_View = glm::lookAt
			(
				transform.GetLocalLocation(),
				transform.GetLocalLocation() + transform.GetForward(),
				transform.GetUp()
			);
	}

	void FreeCameraComponent::UpdateTranslation(TransformComponent& transform)
	{
		using namespace Time;

		const InputManager& inputManager{ InputManager::GetInstance() };

		Math::Vec2D dir{};

		dir.x += inputManager.GetIsKeyPressed(KeyboardInput::A) ? -1.0 : 0.0;
		dir.x += inputManager.GetIsKeyPressed(KeyboardInput::D) ? 1.0 : 0.0;

		dir.y += inputManager.GetIsKeyPressed(KeyboardInput::W) ? 1.0 : 0.0;
		dir.y += inputManager.GetIsKeyPressed(KeyboardInput::S) ? -1.0 : 0.0;

		transform.Translate(transform.GetForward() * dir.y
			* m_Speed * Timer::GetInstance().GetElapsedSeconds(), true);

		transform.Translate(transform.GetRight() * dir.x
			* m_Speed * Timer::GetInstance().GetElapsedSeconds(), true);

		UpdateView(transform);
	}

	void FreeCameraComponent::UpdateRotation(TransformComponent& transform)
	{
		const InputManager& inputManager{ InputManager::GetInstance() };

		const Math::Vec2D& mousePos{ inputManager.GetMousePosition() };
		const Math::Vec2D& lastMousePos{ inputManager.GetPreviousMousePosition() };

		double xOffset{ mousePos.x - lastMousePos.x };
		double yOffset{ lastMousePos.y - mousePos.y };

		const double sensitivity{ 0.001 };

		xOffset *= sensitivity;
		yOffset *= sensitivity;

		transform.Rotate(Math::Vec3D{ yOffset, xOffset, 0.0 }, true);

		UpdateView(transform);
	}

	bool FreeCameraComponent::operator==(const FreeCameraComponent& other) const
	{
		using namespace Math;

		return AreEqual(m_Speed, other.m_Speed) && AreEqual(m_AspectRatio, other.m_AspectRatio) &&
			AreEqual(m_FarPlane, other.m_FarPlane) && AreEqual(m_FOV, other.m_FOV) && AreEqual(m_NearPlane, other.m_NearPlane) &&
			(m_IsActive == other.m_IsActive) && (m_Projection == other.m_Projection) && (m_View == other.m_View);
	}
}