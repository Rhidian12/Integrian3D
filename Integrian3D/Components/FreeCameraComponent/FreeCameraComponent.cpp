#include "FreeCameraComponent.h"

#include "../TransformComponent/TransformComponent.h"
#include "../../SceneManager/SceneManager.h"
#include "../../Math/Math.h"
#include "../../InputManager/InputManager.h"
#include "../../Timer/Timer.h"

#include <gtc/matrix_transform.hpp>
#include <iostream>

namespace Integrian3D
{
	FreeCameraComponent::FreeCameraComponent(const float nearPlane, const float farPlane, const float fov, const float aspectRatio, const float speed)
		: m_NearPlane{ nearPlane }
		, m_FarPlane{ farPlane }
		, m_FOV{ fov }
		, m_AspectRatio{ aspectRatio }
		, m_Speed{ speed }
		, m_View{ 1.f }
		, m_Projection{ 1.f }
	{
		const Math::Vec3D startPos{ 0.f, 0.f, -3.f };
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

		Math::Vec3D dir{};

		dir.x += inputManager.GetIsKeyPressed(KeyboardInput::A) ? -1.f : 0.f;
		dir.x += inputManager.GetIsKeyPressed(KeyboardInput::D) ? 1.f : 0.f;

		dir.z += inputManager.GetIsKeyPressed(KeyboardInput::W) ? 1.f : 0.f;
		dir.z += inputManager.GetIsKeyPressed(KeyboardInput::S) ? -1.f : 0.f;

		dir.y += inputManager.GetIsKeyPressed(KeyboardInput::E) ? 1.f : 0.f;
		dir.y += inputManager.GetIsKeyPressed(KeyboardInput::Q) ? -1.f : 0.f;

		transform.Translate(transform.GetForward() * dir.z
			* m_Speed * Timer::GetInstance().GetElapsedSeconds());

		transform.Translate(transform.GetRight() * dir.x
			* m_Speed * Timer::GetInstance().GetElapsedSeconds());

		transform.Translate(Math::Up * dir.y * m_Speed * Timer::GetInstance().GetElapsedSeconds());

		UpdateView(transform);
	}

	void FreeCameraComponent::UpdateRotation(TransformComponent& transform)
	{
		const InputManager& inputManager{ InputManager::GetInstance() };

		const Math::Vec2D& mousePos{ inputManager.GetMousePosition() };
		const Math::Vec2D& lastMousePos{ inputManager.GetPreviousMousePosition() };

		double xOffset{ mousePos.x - lastMousePos.x };
		double yOffset{ lastMousePos.y - mousePos.y };

		constexpr static float sensitivity{ 0.002f };
		xOffset *= sensitivity;
		yOffset *= sensitivity;

		transform.Rotate(Math::Vec3D{ yOffset, xOffset, 0.f });

		UpdateView(transform);
	}

	void FreeCameraComponent::SetSpeed(const float speed)
	{
		m_Speed = speed;
	}

	bool FreeCameraComponent::operator==(const FreeCameraComponent& other) const
	{
		using namespace Math;

		return AreEqual(m_Speed, other.m_Speed) && AreEqual(m_AspectRatio, other.m_AspectRatio) &&
			AreEqual(m_FarPlane, other.m_FarPlane) && AreEqual(m_FOV, other.m_FOV) && AreEqual(m_NearPlane, other.m_NearPlane) &&
			(m_IsActive == other.m_IsActive) && (m_Projection == other.m_Projection) && (m_View == other.m_View);
	}
}