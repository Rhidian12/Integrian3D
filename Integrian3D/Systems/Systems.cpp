#include "Systems.h"

#include "../SceneManager/SceneManager.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Math/MathUtils.h"
#include "../Timer/Timer.h"

#include <iostream>

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

					std::cout << xOffset << "\n";

					const double sensitivity{ 0.001 };

					xOffset *= sensitivity;
					yOffset *= sensitivity;

					glm::vec3 rot{ MathUtils::ToRadians(xOffset), MathUtils::ToRadians(yOffset), 0.f };

					transform.Rotate(rot, true);

					glm::vec3 newRot{ transform.GetLocalAngle() };

					if (newRot.x < MathUtils::ToRadians(-89.f))
					{
						newRot.x = MathUtils::ToRadians(-89.f);
					}
					else if (newRot.x > MathUtils::ToRadians(89.f))
					{
						newRot.x = MathUtils::ToRadians(89.f);
					}

					transform.SetLocalAngle(newRot);

					rot = glm::vec3
					{
						glm::cos(newRot.y) * glm::cos(newRot.x),
						glm::sin(newRot.x),
						glm::sin(newRot.y) * glm::cos(newRot.x)
					};

					auto test = transform.GetForward();
					auto test2 = glm::normalize(rot);

					camera.SetView(glm::lookAt(transform.GetLocalLocation(), transform.GetLocalLocation() + test, MathUtils::Up));
				}
			);
		}
	}
}