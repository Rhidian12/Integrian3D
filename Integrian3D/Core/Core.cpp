#include "Core.h"

#include "../DebugUtility/DebugUtility.h"
#include "../Timer/Timer.h"

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

		while (g_IsRunning)
		{
			timer.Update();

			/* [TODO]: Update Input */

			/* [TODO]: Update Scenes => Update Entities */

			/* [TODO]: Update Renderer */

			Window.Update();
		}
	}
}