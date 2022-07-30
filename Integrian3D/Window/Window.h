#pragma once

struct GLFWwindow;
namespace Integrian3D
{
	class Window final
	{
	public:
		explicit Window(const int width, const int height);

	};

	namespace
	{
		inline void OnResize(GLFWwindow* pWindow, int width, int height);
	}
}