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
	{
		InputManager::CreateInputManager(&Window);
	}

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

		Math::SetSeed(SEED);

		Debug::LogMessage("Finished initialisation of engine", false);

		return *Instance.get();
	}

	void Core::Run()
	{
		using namespace Time;

		g_IsRunning = true;

		Timer& timer{ Timer::GetInstance() };
		InputManager& inputManager{ InputManager::GetInstance() };
		SceneManager& sceneManager{ SceneManager::GetInstance() };
		Renderer& renderer{ Renderer::GetInstance() };

		for (Scene* const pScene : sceneManager.GetAllScenes())
			pScene->Start();

		double lag{};
		const double timePerFrame{ timer.GetFixedElapsedTime<TimeLength::MilliSeconds>() };

		while (g_IsRunning)
		{
			lag += timer.GetElapsedTime<TimeLength::MilliSeconds>();

			timer.Update();

			inputManager.ProcessInput();

			Scene* const pActiveScene{ sceneManager.GetActiveScene() };

			pActiveScene->Update();

			while (lag >= timePerFrame)
			{
				lag -= timePerFrame;
			}

			renderer.StartRenderLoop(pActiveScene->GetActiveCamera());
			pActiveScene->Render();

			/* Swap buffers */
			Window.Update();

			Debug::LogMessage("FPS: " + std::to_string(Timer::GetInstance().GetFPS()), false);
		}
	}
}