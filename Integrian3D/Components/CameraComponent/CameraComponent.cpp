#include "CameraComponent.h"

#include "../TransformComponent/TransformComponent.h"
#include "../../SceneManager/SceneManager.h"
#include "../../Math/Math.h"
#include "../../InputManager/InputManager.h"
#include "../../Timer/Timer.h"

#include <gtc/matrix_transform.hpp>

namespace Integrian3D
{
	CameraComponent::CameraComponent(GameObject* pOwner, const float nearPlane, const float farPlane, const float fov, const float aspectRatio, const double speed)
		: Component{ pOwner }
		, m_NearPlane{ nearPlane }
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

	void CameraComponent::Start()
	{
		m_View = glm::lookAt(
			m_pOwner->pTransform->GetLocalLocation(),
			m_pOwner->pTransform->GetLocalLocation() + m_pOwner->pTransform->GetForward(),
			m_pOwner->pTransform->GetUp());
	}

	void CameraComponent::Update()
	{
		if (!InputManager::GetInstance().GetIsMouseButtonPressed(MouseInput::RMB))
			return;

		HandleTranslation();
		HandleRotation();
	}

	void CameraComponent::SetView(const glm::mat4& view)
	{
		m_View = view;
	}

	void CameraComponent::SetSpeed(const float speed)
	{
		m_Speed = speed;
	}

	void CameraComponent::HandleTranslation()
	{
		using namespace Time;

		const InputManager& inputManager{ InputManager::GetInstance() };

		Math::Vec2D dir{};

		dir.x += inputManager.GetIsKeyPressed(KeyboardInput::A) ? -1.0 : 0.0;
		dir.x += inputManager.GetIsKeyPressed(KeyboardInput::D) ? 1.0 : 0.0;

		dir.y += inputManager.GetIsKeyPressed(KeyboardInput::W) ? 1.0 : 0.0;
		dir.y += inputManager.GetIsKeyPressed(KeyboardInput::S) ? -1.0 : 0.0;

		m_pOwner->pTransform->Translate(m_pOwner->pTransform->GetForward() * dir.y
			* m_Speed * Timer::GetInstance().GetElapsedSeconds(), true);

		m_pOwner->pTransform->Translate(m_pOwner->pTransform->GetRight() * dir.x
			* m_Speed * Timer::GetInstance().GetElapsedSeconds(), true);

		m_View = glm::lookAt(
			m_pOwner->pTransform->GetLocalLocation(),
			m_pOwner->pTransform->GetLocalLocation() + m_pOwner->pTransform->GetForward(),
			m_pOwner->pTransform->GetUp());
	}

	void CameraComponent::HandleRotation()
	{
		const InputManager& inputManager{ InputManager::GetInstance() };

		const Math::Vec2D& mousePos{ inputManager.GetMousePosition() };
		const Math::Vec2D& lastMousePos{ inputManager.GetPreviousMousePosition() };

		double xOffset{ mousePos.x - lastMousePos.x };
		double yOffset{ lastMousePos.y - mousePos.y };

		const double sensitivity{ 0.001 };

		xOffset *= sensitivity;
		yOffset *= sensitivity;

		m_pOwner->pTransform->Rotate(Math::Vec3D{ yOffset, xOffset, 0.0 }, true);

		m_View = glm::lookAt(m_pOwner->pTransform->GetLocalLocation(),
			m_pOwner->pTransform->GetLocalLocation() + m_pOwner->pTransform->GetForward(),
			m_pOwner->pTransform->GetUp());
	}
}