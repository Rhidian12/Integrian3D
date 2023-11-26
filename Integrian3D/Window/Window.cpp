#include "Window.h"

#include "DebugUtility.h"
#include "InputManager/InputManager.h"
#include "Math/Math.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#ifdef _WIN32

#	include "Win32Utils/Win32APICall.h"

I_DISABLE_WARNING(4005) /* warning C4005: 'APIENTRY': macro redefinition */
#		define WIN32_LEAN_AND_MEAN
#		include <Windows.h>

I_ENABLE_WARNING(4005)

#endif

namespace Integrian3D
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

		// [CRINGE]: Window should not be responsible for all of this initialization 
		#if _DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
		#endif

		pWindow = glfwCreateWindow(width, height, "Integrian3D", nullptr, nullptr);

		if (!pWindow)
		{
			LOG(WindowLog, LogErrorLevel::Fatal, "Failed to create GLFW window");

			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(pWindow);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LOG(WindowLog, LogErrorLevel::Fatal, "Failed to initialize GLAD");
			return;
		}

		glViewport(0, 0, width, height);

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(pWindow, true);

		CALL_WIN32_IGNORE_ERROR(ImGui_ImplOpenGL3_Init(), ERROR_PROC_NOT_FOUND);

		glfwSetFramebufferSizeCallback(pWindow, OnResize);

		glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// glfwSetCursorPosCallback(pWindow, OnMouseMovement);
	}

	Window::~Window()
	{
		// I'm not sure why, but this call produces ERROR_FILE_NOT_FOUND
		CALL_WIN32_IGNORE_ERROR(glfwTerminate(), ERROR_FILE_NOT_FOUND);
	}

	void Window::Update()
	{
		__CHECK(pWindow != nullptr);

		glfwSwapBuffers(pWindow);

		g_IsRunning = !glfwWindowShouldClose(pWindow);
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
			InputManager::GetInstance().SetMousePosition(Math::Vec2D{ x, y });
		}
	}
}
