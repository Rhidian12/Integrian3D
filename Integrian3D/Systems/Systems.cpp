#include "Systems.h"

#include "../SceneManager/SceneManager.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Math/MathUtils.h"
#include "../Timer/Timer.h"
#include "../DebugUtility/DebugUtility.h"

#include <iostream>
#include <iomanip>

namespace Integrian3D
{
	namespace Systems
	{
		void TranslateCamera(const glm::vec3& dir)
		{
			SceneManager::GetInstance().GetActiveScene().CreateView<CameraComponent, TransformComponent>().ForEach(
				[&dir](CameraComponent& camera, TransformComponent& transform)->void
				{
					transform.Translate(dir * camera.GetSpeed() * Timer::GetInstance().GetElapsedSeconds(), true);
					
					/* Make lookat matrix */
					glm::mat4 lookAt{ glm::lookAt(transform.GetLocalLocation(), transform.GetLocalLocation() + transform.GetForward(), MathUtils::Up)};

					camera.SetView(lookAt);

					///* Get camera direction, which is the inverse of what we're looking at */
					//const glm::vec3 cameraInverseDir{ glm::normalize(transform.GetLocalLocation()) };

					///* Get the camera right axis */
					//const glm::vec3 cameraRight{ glm::normalize(glm::cross(MathUtils::Up, cameraInverseDir)) };

					///* Get the camera up axis */
					//const glm::vec3 cameraUp{ glm::cross(cameraInverseDir, cameraRight) }; 
				}
			);
		}

		void RotateCamera(const MathUtils::Vec2D& mousePos, const MathUtils::Vec2D& lastMousePos)
		{
			SceneManager::GetInstance().GetActiveScene().CreateView<CameraComponent, TransformComponent>().ForEach(
				[&mousePos, &lastMousePos](CameraComponent& camera, TransformComponent& transform)->void
				{
					double xOffset{ mousePos.x - lastMousePos.x };
					double yOffset{ mousePos.y - lastMousePos.y };

					const double sensitivity{ 1.0 };

					xOffset *= sensitivity * Timer::GetInstance().GetElapsedSeconds() * 10.0;
					yOffset *= sensitivity * Timer::GetInstance().GetElapsedSeconds() * 10.0;

					transform.Rotate(MathUtils::Vec3D{ yOffset, xOffset, 0.0 }, true);

					camera.SetView(glm::lookAt(transform.GetLocalLocation(), transform.GetLocalLocation() + transform.GetForward(), MathUtils::Up));
				}
			);
		}
	}
}