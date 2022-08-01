#pragma once

#include "../Window/Window.h"

namespace Integrian3D
{
	static volatile bool g_IsRunning{ true };

	class Core final
	{
	public:
		explicit Core(const int windowWidth, const int windowHeight);

		Core(const Core&) noexcept = delete;
		Core(Core&&) noexcept = delete;
		Core& operator=(const Core&) noexcept = delete;
		Core& operator=(Core&&) noexcept = delete;

		void Run();

	private:
		Detail::Window Window;
	};
}