#pragma once

#include "EngineConstants.h"
#include "Window/Window.h"

#include <memory> /* std::unique_ptr */

DEFINE_LOG_CATEGORY(CoreLog, Integrian3D::LogVerbosity::Verbose)

namespace Integrian3D
{
	class Core final
	{
	public:
		static Core& GetInstance();
		static Core& CreateCore(const int windowWidth, const int windowHeight);
			
		Core(const Core&) noexcept = delete;
		Core(Core&&) noexcept = delete;
		Core& operator=(const Core&) noexcept = delete;
		Core& operator=(Core&&) noexcept = delete;

		void Run();

		__NODISCARD int GetWindowWidth() const { return Window.GetWidth(); }
		__NODISCARD int GetWindowHeight() const { return Window.GetHeight(); }
		__NODISCARD const Detail::Window& GetWindow() const { return Window; }

	private:
		Core(const int windowWidth, const int windowHeight);
		void Shutdown();

		Detail::Window Window;

		inline static std::unique_ptr<Core> Instance{};
	};
}