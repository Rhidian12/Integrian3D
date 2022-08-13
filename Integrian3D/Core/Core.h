#pragma once

#include "../EngineConstants.h"
#include "../Window/Window.h"

namespace Integrian3D
{
	inline volatile bool g_IsRunning{ true };

	class Core final
	{
	public:
		Core(const int windowWidth, const int windowHeight);

		Core(const Core&) noexcept = delete;
		Core(Core&&) noexcept = delete;
		Core& operator=(const Core&) noexcept = delete;
		Core& operator=(Core&&) noexcept = delete;

		void Run();

		__NODISCARD int GetWindowWidth() const { return Window.GetWidth(); }
		__NODISCARD int GetWindowHeight() const { return Window.GetHeight(); }

	private:
		Detail::Window Window;
	};
}