#include "IO/File/FileMonitor.h"

#include "IO/PathUtils.h"
#include "IO/OpenMode.h"
#include "Thread/ThreadManager.h"
#include "Timer/TimeLength.h"
#include "Win32Utils/Win32APICall.h"

#ifdef _WIN32
I_DISABLE_WARNING(4005) // warning C4005: 'APIENTRY': macro redefinition
#include <Windows.h>
I_ENABLE_WARNING(4005)
#endif

namespace Integrian3D::IO
{
	namespace
	{
		static int64 GetLastTimeModified(const char* Filepath)
		{
			struct _stat Result;
			if (_stat(Filepath, &Result) == 0)
			{
				return Result.st_mtime;
			}

			return 0;
		}
	}

	FileMonitor::FileMonitor(const std::string& Path)
		: Filepath{ Path }
		, bIsMonitoring{ false }
		, LastTimeModified{ -1 }
		, ThreadID{ -1 }
	{}

	FileMonitor::~FileMonitor()
	{
		bIsMonitoring = false;

		if (ThreadID != -1)
		{
			Threading::ThreadManager::GetInstance().WaitOnThread(ThreadID);
		}
	}

	void FileMonitor::StartMonitoringFile()
	{
		bIsMonitoring = true;

		LastTimeModified = GetLastTimeModified(Filepath.c_str());
		if (!LastTimeModified)
		{
			LOG(FileMonitorLog, LogErrorLevel::Error, "Could not retrieve LastTimeModified for file {}. Did not start monitoring file!", Filepath);
			return;
		}

		ThreadID = Threading::ThreadManager::ScheduleTask([this]()->void
			{
				while (bIsMonitoring)
				{
					constexpr static int32 NrOfMSToWait{ 33 };
					std::this_thread::sleep_for(std::chrono::milliseconds(NrOfMSToWait));

					const int64 LastModified = GetLastTimeModified(Filepath.c_str());

					if (LastModified > LastTimeModified)
					{
						OnFileChanged.Invoke(Filepath);
						LastTimeModified = LastModified;
					}
				}
			});
	}

	void FileMonitor::StopMonitoringFile()
	{
		bIsMonitoring = false;
	}

	Delegate<std::string>& FileMonitor::GetOnFileChangedDelegate()
	{
		return OnFileChanged;
	}
}