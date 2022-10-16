#include "Window.h"

#include "../Core/Core.h" /* g_IsRunning */
#include "../DebugUtility/DebugUtility.h"
#include "../InputManager/InputManager.h"

namespace Integrian3D
{
	/* Defined in Core.h */
	extern inline volatile bool g_IsRunning;

	namespace Detail
	{
		Window::Window(const int width, const int height)
			: pWindow{}
			, Width{ width }
			, Height{ height }
		{
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			pWindow = glfwCreateWindow(width, height, "Integrian3D", nullptr, nullptr);

			if (!pWindow)
			{
				Debug::LogError("Failed to create GLFW window", false);

				glfwTerminate();
				return;
			}

			glfwMakeContextCurrent(pWindow);

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				Debug::LogError("Failed to initialize GLAD", false);

				return;
			}

			glViewport(0, 0, width, height);

			glfwSetFramebufferSizeCallback(pWindow, OnResize);

			glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			glfwSetCursorPosCallback(pWindow, OnMouseMovement);
		}

		Window::~Window()
		{
			glfwTerminate();
		}

		void Window::Update()
		{
			glfwSwapBuffers(pWindow);

			g_IsRunning = !glfwWindowShouldClose(pWindow);
		}
	}

	namespace
	{
		/* [TODO]: Window::Width and Window::Height are not getting changed! */
		void OnResize(GLFWwindow*, int width, int height)
		{
			glViewport(0, 0, width, height);
		}

		void OnMouseMovement(GLFWwindow*, double x, double y)
		{
			InputManager::GetInstance().SetMousePosition(MathUtils::Vec2D{ x, y });
		}
	}
}
