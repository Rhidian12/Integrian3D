#include "Core.h"

#include "DebugUtility/DebugUtility.h"
#include "Timer/Timer.h"
#include "InputManager/InputManager.h"
#include "SceneManager/SceneManager.h"
#include "Components/MeshComponent/MeshComponent.h"
#include "Components/TransformComponent/TransformComponent.h"

#include <raylib.h>

namespace Integrian3D
{
	Core::Core(const int windowWidth, const int windowHeight)
		: Window{ windowWidth, windowHeight }
	{
		srand(static_cast<unsigned int>(time(nullptr)));

		Math::SetSeed(SEED);

		LOG(CoreLog, LogErrorLevel::Log, "Finished initialisation of engine");
	}

	Core::~Core()
	{
		Logger::Cleanup();
	}

	void Core::Run()
	{
		using namespace Time;

		g_IsRunning = true;

		Timer& timer{ Timer::GetInstance() };
		InputManager& inputManager{ InputManager::GetInstance() };
		SceneManager& sceneManager{ SceneManager::GetInstance() };

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

			pActiveScene->Render();

			/* Swap buffers */
			Window.Update();

			// LogMessage("FPS: " + std::to_string(Timer::GetInstance().GetFPS()), false);
		}
	}
}