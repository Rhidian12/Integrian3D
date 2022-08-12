#include "Core.h"

#include "../DebugUtility/DebugUtility.h"
#include "../Timer/Timer.h"
#include "../InputManager/InputManager.h"
#include "../Renderer/Renderer.h"
#include "../SceneManager/SceneManager.h"
#include "../Components/MeshComponent/MeshComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"

#include <gtc/matrix_transform.hpp>

namespace Integrian3D
{
	Core::Core(const int windowWidth, const int windowHeight)
		: Window{ windowWidth, windowHeight }
	{
		Debug::LogMessage("Finished initialisation of engine", false);
	}

	void Core::Run()
	{
		Timer& timer{ Timer::GetInstance() };
		InputManager& inputManager{ InputManager::GetInstance() };
		Renderer& renderer{ Renderer::GetInstance() };
		SceneManager& sceneManager{ SceneManager::GetInstance() };

		while (g_IsRunning)
		{
			timer.Update();

			inputManager.ProcessInput();

			Scene& activeScene{ sceneManager.GetActiveScene() };

			/* Entity update */
			{
				{
					/* [TODO]: add user defined updates here */
				}

				/* Transform update */
				{
					View<TransformComponent> transformView{ activeScene.CreateView<TransformComponent>() };
					transformView.ForEach([](TransformComponent& transformComponent)->void
						{
							if (transformComponent.bShouldRecalculateTransform)
							{
								glm::mat4 translationMatrix{ 1.f };
								translationMatrix[3] = glm::vec4{ transformComponent.GetLocalLocation(), 1.f };

								const glm::vec3& rotation{ transformComponent.GetLocalAngle() };
								glm::mat4 rotationMatrix{ 1.f };
								rotationMatrix = glm::rotate(rotationMatrix, rotation.x, glm::vec3{ 1.f, 0.f, 0.f });
								rotationMatrix = glm::rotate(rotationMatrix, rotation.y, glm::vec3{ 0.f, 1.f, 0.f });
								rotationMatrix = glm::rotate(rotationMatrix, rotation.z, glm::vec3{ 0.f, 0.f, 1.f });

								glm::mat4 scaleMatrix{ 1.f };
								scaleMatrix = glm::scale(scaleMatrix, transformComponent.GetLocalScale());

								transformComponent.Transformation = translationMatrix * rotationMatrix * scaleMatrix;
							}
						});
				}
			}

			/* Rendering */
			{
				renderer.StartRenderLoop();
				View<MeshComponent> renderView{ activeScene.CreateView<MeshComponent>() };
				renderView.ForEach([&renderer](const MeshComponent& meshComponent)->void
					{
						renderer.Render(meshComponent);
					});
			}

			Window.Update();
		}
	}
}