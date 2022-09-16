#include "Core.h"

#include "../DebugUtility/DebugUtility.h"
#include "../Timer/Timer.h"
#include "../InputManager/InputManager.h"
#include "../Renderer/Renderer.h"
#include "../SceneManager/SceneManager.h"
#include "../Components/MeshComponent/MeshComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"

#include <gtc/matrix_transform.hpp>

#pragma warning( push )
#pragma warning( disable : 4201 )
#include <gtx/euler_angles.hpp>
#pragma warning( pop )

namespace Integrian3D
{
	Core::Core(const int windowWidth, const int windowHeight)
		: Window{ windowWidth, windowHeight }
	{}

	Core& Core::GetInstance()
	{
		__ASSERT(Instance != nullptr && "Core::GetInstance() > No core has been created");

		return *Instance.get();
	}

	Core& Core::CreateCore(const int windowWidth, const int windowHeight)
	{
		__ASSERT(Instance == nullptr && "Core::CreateCore() > A core has already been created");

		Instance = std::unique_ptr<Core>(new Core{ windowWidth, windowHeight });

		srand(static_cast<unsigned int>(time(nullptr)));

		MathUtils::Seed = SEED;

		Debug::LogMessage("Finished initialisation of engine", false);

		return *Instance.get();
	}

	void Core::Run()
	{
		Timer& timer{ Timer::GetInstance() };
		InputManager& inputManager{ InputManager::GetInstance() };
		Renderer& renderer{ Renderer::GetInstance() };
		SceneManager& sceneManager{ SceneManager::GetInstance() };

		for (Scene& scene : sceneManager.GetAllScenes())
		{
			scene.Start();
		}

		while (g_IsRunning)
		{
			timer.Update();

			inputManager.ProcessInput();

			Scene& activeScene{ sceneManager.GetActiveScene() };

			/* Entity update */
			{
				/* User-defined update */
				{
					for (const std::function<void(Scene&)>& fn : activeScene.GetUpdateCallbacks())
					{
						fn(activeScene);
					}
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
								transformationMatrix *= glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);

								
								transformationMatrix *= glm::scale(transformationMatrix, transformComponent.GetLocalScale());

								transformComponent.Transformation = transformationMatrix;
							}
						});
				}
			}

			/* Rendering */
			{
				renderer.StartRenderLoop(activeScene.GetCamera());
				View<MeshComponent, TransformComponent> renderView{ activeScene.CreateView<MeshComponent, TransformComponent>() };
				renderView.ForEach([&renderer](const MeshComponent& meshComponent, const TransformComponent& transformComponent)->void
					{
						renderer.Render(meshComponent, transformComponent);
					});
			}

			/* Swap buffers */
			Window.Update();
		}
	}
}