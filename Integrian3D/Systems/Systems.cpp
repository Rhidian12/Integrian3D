#include "Systems.h"

#include "../SceneManager/SceneManager.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Math/Math.h"
#include "../Timer/Timer.h"
#include "../DebugUtility/DebugUtility.h"
#include "../InputManager/InputManager.h"

namespace Integrian3D
{
	namespace Systems
	{
		void TranslateCamera()
		{
			SceneManager::GetInstance().GetActiveScene().CreateView<CameraComponent, TransformComponent>().ForEach(
				[](CameraComponent& camera, TransformComponent& transform)->void
				{
					Math::Vec2D dir{};

					dir.x += InputManager::GetInstance().GetIsKeyPressed(KeyboardInput::A) ? -1.0 : 0.0;
					dir.x += InputManager::GetInstance().GetIsKeyPressed(KeyboardInput::D) ? 1.0 : 0.0;

					dir.y += InputManager::GetInstance().GetIsKeyPressed(KeyboardInput::W) ? 1.0 : 0.0;
					dir.y += InputManager::GetInstance().GetIsKeyPressed(KeyboardInput::S) ? -1.0 : 0.0;

					transform.Translate(transform.GetForward() * dir.y
						* camera.GetSpeed() * Timer::GetInstance().GetElapsedSeconds(), true);

					transform.Translate(transform.GetRight() * dir.x
						* camera.GetSpeed() * Timer::GetInstance().GetElapsedSeconds(), true);

					camera.SetView(glm::lookAt(
						transform.GetLocalLocation(),
						transform.GetLocalLocation() + transform.GetForward(),
						transform.GetUp())
					);
				}
			);
		}

		void RotateCamera()
		{
			const Math::Vec2D& mousePos{ InputManager::GetInstance().GetMousePosition() };
			const Math::Vec2D& lastMousePos{ InputManager::GetInstance().GetPreviousMousePosition() };

			SceneManager::GetInstance().GetActiveScene().CreateView<CameraComponent, TransformComponent>().ForEach(
				[&mousePos, &lastMousePos](CameraComponent& camera, TransformComponent& transform)->void
				{
					double xOffset{ mousePos.x - lastMousePos.x };
					double yOffset{ lastMousePos.y - mousePos.y };

					const double sensitivity{ 0.001 };

					xOffset *= sensitivity;
					yOffset *= sensitivity;

					transform.Rotate(Math::Vec3D{ yOffset, xOffset, 0.0 }, true);

					camera.SetView(
						glm::lookAt(transform.GetLocalLocation(),
							transform.GetLocalLocation() + transform.GetForward(),
							transform.GetUp())
					);
				}
			);
		}
	}
}