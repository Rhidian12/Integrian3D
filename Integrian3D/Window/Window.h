#pragma once

#include "../EngineConstants.h"

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

		private:
			GLFWwindow* pWindow;
			int Width;
			int Height;
		};
	}

	namespace
	{
		/* Ignore the static compiler, this function is defined in Window.cpp */
		inline void OnResize(GLFWwindow* pWindow, int width, int height);
	}
}