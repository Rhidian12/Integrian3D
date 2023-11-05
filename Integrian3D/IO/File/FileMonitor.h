#pragma once

#include "EngineConstants.h"

#include "Array/Array.h"
#include "Delegate/Delegate.h"
#include "TPair/TPair.h"

#include <functional>
#include <mutex>
#include <string>

DEFINE_LOG_CATEGORY(FileMonitorLog, Integrian3D::LogVerbosity::Verbose);

namespace Integrian3D::IO
{
	class FileMonitor
	{
	public:
		FileMonitor();
		~FileMonitor();

		void StopMonitor();

		void StartMonitoringFile(const std::string& Filepath);
		void StopMonitoringFile(const std::string& Filepath);

		void BindToOnFileChanged(const std::function<void(std::string)>& Callback);

	private:
		TArray<TPair<std::string, int64>> Filepaths;
		Delegate<std::string> OnFileChanged;
		std::mutex Mutex;
		int32 ThreadID;
		bool bIsMonitoring;
	};
}