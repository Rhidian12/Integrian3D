#include "Systems.h"

#include "../SceneManager/SceneManager.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Math/MathUtils.h"
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
					MathUtils::Vec2D dir{};

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
			const MathUtils::Vec2D& mousePos{ InputManager::GetInstance().GetMousePosition() };
			const MathUtils::Vec2D& lastMousePos{ InputManager::GetInstance().GetPreviousMousePosition() };

			SceneManager::GetInstance().GetActiveScene().CreateView<CameraComponent, TransformComponent>().ForEach(
				[&mousePos, &lastMousePos](CameraComponent& camera, TransformComponent& transform)->void
				{
					double xOffset{ mousePos.x - lastMousePos.x };
					double yOffset{ mousePos.y - lastMousePos.y };

					const double speed{ 5.0 };

					xOffset *= speed * Timer::GetInstance().GetElapsedSeconds();
					yOffset *= speed * Timer::GetInstance().GetElapsedSeconds();

					transform.Rotate(MathUtils::Vec3D{ yOffset, xOffset, 0.0 }, true);

					Debug::LogVector(transform.GetLocalAngle(), Debug::MessageColour::White);

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