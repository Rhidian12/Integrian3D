#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Core/Core.h" /* g_IsRunning */
#include "../DebugUtility/DebugUtility.h"

namespace Integrian3D
{
	/* Defined in Core.h */
	extern inline volatile bool g_IsRunning;

	namespace Detail
	{
		Window::Window(const int width, const int height)
			: pWindow{}
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
		}

		Window::~Window()
		{
			glfwTerminate();
		}

		void Window::Update()
		{
			glfwSwapBuffers(pWindow);

			g_IsRunning = glfwWindowShouldClose(pWindow);
		}
	}

	namespace
	{
		void OnResize(GLFWwindow*, int width, int height)
		{
			glViewport(0, 0, width, height);
		}
	}
}
