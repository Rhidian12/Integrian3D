#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace Integrian3D
{
	Window::Window(const int width, const int height)
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLFWwindow* pWindow = glfwCreateWindow(width, height, "Integrian3D", nullptr, nullptr);

		if (!pWindow)
		{
			/* [TODO]: Replace this with logger */
			std::cout << "Failed to create GLFW window\n";

			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(pWindow);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			/* [TODO]: Replace this with logger */
			std::cout << "Failed to initialize GLAD\n";
			return;
		}

		glViewport(0, 0, width, height);

		glfwSetFramebufferSizeCallback(pWindow, OnResize);
	}

	namespace
	{
		void OnResize(GLFWwindow*, int width, int height)
		{
			glViewport(0, 0, width, height);
		}
	}
}
