#pragma once

struct GLFWwindow;
namespace Integrian3D
{
	namespace Detail
	{
		class Window final
		{
		public:
			explicit Window(const int width, const int height);

			/* [TODO]: Probably needs a destructor to close GLFW correctly */

			Window(const Window&) noexcept = delete;
			Window(Window&&) noexcept = delete;
			Window& operator=(const Window&) noexcept = delete;
			Window& operator=(Window&&) noexcept = delete;

			void Update();

		private:
			GLFWwindow* pWindow;
		};
	}

	namespace
	{
		/* Ignore the static compiler, this function is defined in Window.cpp */
		inline void OnResize(GLFWwindow* pWindow, int width, int height);
	}
}