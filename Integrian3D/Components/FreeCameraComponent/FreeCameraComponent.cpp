#include "FreeCameraComponent.h"

#include "Components/TransformComponent/TransformComponent.h"
#include "SceneManager/SceneManager.h"
#include "Math/Math.h"
#include "Math/GlmToRay.h"
#include "InputManager/InputManager.h"
#include "Timer/Timer.h"

#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <raylib.h>

namespace Integrian3D
{
	FreeCameraComponent::FreeCameraComponent(const float FOV, const double Speed)
		: CameraImpl{ MakeUnique<Camera3D>(ToRay3(Math::Vec3D{}), ToRay3(Math::Vec3D{}), ToRay3(Math::Up), FOV, CAMERA_PERSPECTIVE) }
		, Speed{ Speed }
	{}

	void FreeCameraComponent::UpdateTranslation(TransformComponent& transform)
	{
		using namespace Time;

		const InputManager& inputManager{ InputManager::GetInstance() };

		Math::Vec3D dir{};

		dir.x += inputManager.GetIsKeyPressed(KeyboardInput::A) ? -1.0 : 0.0;
		dir.x += inputManager.GetIsKeyPressed(KeyboardInput::D) ? 1.0 : 0.0;

		dir.z += inputManager.GetIsKeyPressed(KeyboardInput::W) ? 1.0 : 0.0;
		dir.z += inputManager.GetIsKeyPressed(KeyboardInput::S) ? -1.0 : 0.0;

		dir.y += inputManager.GetIsKeyPressed(KeyboardInput::E) ? 1.0 : 0.0;
		dir.y += inputManager.GetIsKeyPressed(KeyboardInput::Q) ? -1.0 : 0.0;

		transform.Translate(transform.GetForward() * dir.z
			* Speed * Timer::GetInstance().GetElapsedSeconds());

		transform.Translate(transform.GetRight() * dir.x
			* Speed * Timer::GetInstance().GetElapsedSeconds());

		transform.Translate(Math::Up * dir.y * Speed * Timer::GetInstance().GetElapsedSeconds());

		Math::Vec3D eulerAngles{ glm::eulerAngles(transform.GetLocalAngle()) };
		eulerAngles.x = Math::ToDegrees(eulerAngles.x);
		eulerAngles.y = Math::ToDegrees(eulerAngles.y);
		eulerAngles.z = Math::ToDegrees(eulerAngles.z);

		UpdateCameraPro(CameraImpl.Get(), ToRay3(transform.GetLocalLocation()), ToRay3(eulerAngles), 1.f);
	}

	void FreeCameraComponent::UpdateRotation(TransformComponent& transform)
	{
		const InputManager& inputManager{ InputManager::GetInstance() };

		const Math::Vec2D& mousePos{ inputManager.GetMousePosition() };
		const Math::Vec2D& lastMousePos{ inputManager.GetPreviousMousePosition() };

		double xOffset{ mousePos.x - lastMousePos.x };
		double yOffset{ lastMousePos.y - mousePos.y };

		constexpr static double sensitivity{ 0.002 };
		xOffset *= sensitivity;
		yOffset *= sensitivity;

		transform.Rotate(Math::Vec3D{ yOffset, xOffset, 0.0 });

		Math::Vec3D eulerAngles{ glm::eulerAngles(transform.GetLocalAngle()) };
		eulerAngles.x = Math::ToDegrees(eulerAngles.x);
		eulerAngles.y = Math::ToDegrees(eulerAngles.y);
		eulerAngles.z = Math::ToDegrees(eulerAngles.z);

		UpdateCameraPro(CameraImpl.Get(), ToRay3(transform.GetLocalLocation()), ToRay3(eulerAngles), 1.f);
	}

	void FreeCameraComponent::SetSpeed(const double NewSpeed)
	{
		Speed = NewSpeed;
	}

	double FreeCameraComponent::GetSpeed() const
	{
		return Speed;
	}

	const Camera3D* FreeCameraComponent::GetRayLibCamera() const
	{
		return CameraImpl.Get();
	}
}