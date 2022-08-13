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

	Core& Core::GetInstance()
	{
		return *Instance.get();
	}

	Core& Core::CreateCore(const int windowWidth, const int windowHeight)
	{
		if (!Instance)
		{
			Instance = std::unique_ptr<Core>(new Core{ windowWidth, windowHeight });
		}

		return *Instance.get();
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
								glm::mat4 transformationMatrix{ 1.f };
								transformationMatrix = glm::translate(transformationMatrix, transformComponent.GetLocalLocation());

								const glm::vec3& rotation{ transformComponent.GetLocalAngle() };
								transformationMatrix = glm::rotate(transformationMatrix, rotation.x, glm::vec3{ 1.f, 0.f, 0.f });
								transformationMatrix = glm::rotate(transformationMatrix, rotation.y, glm::vec3{ 0.f, 1.f, 0.f });
								transformationMatrix = glm::rotate(transformationMatrix, rotation.z, glm::vec3{ 0.f, 0.f, 1.f });

								transformationMatrix = glm::scale(transformationMatrix, transformComponent.GetLocalScale());

								transformComponent.Transformation = transformationMatrix;
							}
						});
				}
			}

			/* Rendering */
			{
				renderer.StartRenderLoop();
				View<MeshComponent, TransformComponent> renderView{ activeScene.CreateView<MeshComponent, TransformComponent>() };
				renderView.ForEach([&renderer](const MeshComponent& meshComponent, const TransformComponent& transformComponent)->void
					{
						renderer.Render(meshComponent, transformComponent);
					});
			}

			Window.Update();
		}
	}
}