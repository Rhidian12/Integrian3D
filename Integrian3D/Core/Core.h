#pragma once

#include "EngineConstants.h"

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

	private:
		Core();
		void Shutdown();
	};
}