#pragma once

#include "EngineConstants.h"
#include "Delegate/Delegate.h"
#include "Win32Utils/Win32Handle.h"

#include <string>

DEFINE_LOG_CATEGORY(FileMonitorLog, Integrian3D::LogVerbosity::Verbose);

namespace Integrian3D::IO
{
	class FileMonitor
	{
	public:
		FileMonitor(const std::string& Path);

		void StartMonitoringFile();
		void StopMonitoringFile();

		Delegate<std::string>& GetOnFileChangedDelegate();

	private:
		Win32Utils::Win32Handle Handle;
		std::string Filepath;
		Delegate<std::string> OnFileChanged;
		bool bIsMonitoring;
	};
}