#pragma once

class GLFWwindow;
namespace Integrian3D
{
	class Window final
	{
	public:
		explicit Window(const int width, const int height);

		void OnResize(GLFWwindow* pWindow, const int width, const int height);
	};
}