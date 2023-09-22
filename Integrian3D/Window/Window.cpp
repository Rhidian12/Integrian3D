#include "Window.h"

#include "Core/Core.h" /* g_IsRunning */
#include "DebugUtility/DebugUtility.h"
#include "InputManager/InputManager.h"
#include "Math/Math.h"

#include <raylib.h>

namespace Integrian3D
{
	/* Defined in Core.h */
	extern inline volatile bool g_IsRunning;

	namespace Detail
	{
		Window::Window(const int width, const int height)
			: Width{ width }
			, Height{ height }
		{
			InitWindow(width, height, "Integrian3D");
		}

		Window::~Window()
		{
			CloseWindow();
		}

		void Window::Update()
		{
			EndDrawing();

			g_IsRunning = !WindowShouldClose();
		}
	}
}
