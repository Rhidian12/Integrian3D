#pragma once

#include "EngineConstants.h"
#include "Delegate/Delegate.h"

#include <string>

DEFINE_LOG_CATEGORY(FileMonitorLog, Integrian3D::LogVerbosity::Verbose);

namespace Integrian3D::IO
{
	class FileMonitor
	{
	public:
		FileMonitor(const std::string& Path);
		~FileMonitor();

		void StartMonitoringFile();
		void StopMonitoringFile();

		Delegate<std::string>& GetOnFileChangedDelegate();

	private:
		std::string Filepath;
		Delegate<std::string> OnFileChanged;
		int64 LastTimeModified;
		int32 ThreadID;
		bool bIsMonitoring;
	};
}