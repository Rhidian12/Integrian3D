#include "Core.h"

#include "../DebugUtility/DebugUtility.h"
#include "../Timer/Timer.h"
#include "../InputManager/InputManager.h"
#include "../Renderer/Renderer.h"
#include "../SceneManager/SceneManager.h"

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

			/* [TODO]: Add Scene Update! */

			renderer.Render();

			Window.Update();
		}
	}
}