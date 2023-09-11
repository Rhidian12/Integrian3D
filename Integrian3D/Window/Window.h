#pragma once

#include "EngineConstants.h"

DECLARE_LOG_CATEGORY(Window)

struct GLFWwindow;
namespace Integrian3D
{
	namespace Detail
	{
		class Window final
		{
		public:
			Window(const int width, const int height);
			~Window();

			Window(const Window&) noexcept = delete;
			Window(Window&&) noexcept = delete;
			Window& operator=(const Window&) noexcept = delete;
			Window& operator=(Window&&) noexcept = delete;

			void Update();

			__NODISCARD int GetWidth() const { return Width; }
			__NODISCARD int GetHeight() const { return Height; }
			__NODISCARD GLFWwindow* const GetWindow() const { return pWindow; }

		private:
			GLFWwindow* pWindow;
			int Width;
			int Height;
		};
	}

	namespace
	{
		/* Ignore intellisense, this function is defined in Window.cpp */
		inline void OnResize(GLFWwindow* pWindow, int width, int height);

		/* Ignore intellisense, this function is defined in Window.cpp */
		inline void OnMouseMovement(GLFWwindow* pWindow, double x, double y);
	}
}