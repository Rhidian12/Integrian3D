#include "Core.h"

#include "../DebugUtility/DebugUtility.h"
#include "../Timer/Timer.h"
#include "../InputManager/InputManager.h"
#include "../Renderer/Renderer.h"
#include "../SceneManager/SceneManager.h"
#include "../Components/MeshComponent/MeshComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"

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
								const Matrix4f translationMatrix{};

								const Point3f& localAngle{ transformComponent.GetLocalAngle() };
								const Matrix4f rotationMatrix{ Matrix4f::MakeRotationMatrix(localAngle.X, localAngle.Y, localAngle.Z) };

								const Point3f& localScale{ transformComponent.GetLocalScale() };
								Matrix4f scaleMatrix{ Matrix4f::MakeScaleMatrix(localScale.X, localScale.Y, localScale.Z) };

								transformComponent.SetTransform(translationMatrix * rotationMatrix * scaleMatrix);

								transformComponent.bShouldRecalculateTransform = false;
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