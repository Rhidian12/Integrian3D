#include "Systems.h"

#include "../SceneManager/SceneManager.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Math/MathUtils.h"

namespace Integrian3D
{
	namespace Systems
	{
		void MoveCamera(const glm::vec3& trans)
		{
			SceneManager::GetInstance().GetActiveScene().CreateView<CameraComponent, TransformComponent>().ForEach(
				[&trans](CameraComponent& camera, TransformComponent& transform)->void
				{
					transform.Translate(trans, true);

					/* Get camera direction, which is the inverse of what we're looking at */
					const glm::vec3 cameraInverseDir{ glm::normalize(transform.GetLocalLocation()) };

					/* Get the camera right axis */
					const glm::vec3 cameraRight{ glm::normalize(glm::cross(MathUtils::Up, cameraInverseDir)) };

					/* Get the camera up axis */
					const glm::vec3 cameraUp{ glm::cross(cameraInverseDir, cameraRight) };

					/* Make lookat matrix */
					glm::mat4 lookAt{ glm::lookAt(transform.GetLocalLocation(), cameraInverseDir, MathUtils::Up)};

					camera.SetView(lookAt);
				}
			);
		}
	}
}