#include "Systems.h"

#include "../SceneManager/SceneManager.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Math/MathUtils.h"
#include "../Timer/Timer.h"

namespace Integrian3D
{
	namespace Systems
	{
		void TransformCamera(const glm::vec3& dir, const glm::vec3& rot)
		{
			SceneManager::GetInstance().GetActiveScene().CreateView<CameraComponent, TransformComponent>().ForEach(
				[&dir, &rot](CameraComponent& camera, TransformComponent& transform)->void
				{
					transform.Translate(dir * camera.GetSpeed() * Timer::GetInstance().GetElapsedSeconds(), true);
					transform.Rotate(rot, true);

					/* Make lookat matrix */
					glm::mat4 lookAt{ glm::lookAt(transform.GetLocalLocation(), transform.GetLocalLocation() + MathUtils::Forward, MathUtils::Up) };

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
	}
}