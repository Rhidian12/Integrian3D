#include "IO/File/FileMonitor.h"

#include "IO/PathUtils.h"
#include "IO/OpenMode.h"

#include "Thread/ThreadManager.h"

#include "Timer/TimeLength.h"

#include "TPair/TPair.full.h"

#include "Win32Utils/Win32APICall.h"

#define ACQUIRE_LOCK() const std::unique_lock<std::mutex> Lock{ Mutex }

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

	FileMonitor::FileMonitor()
		: Filepaths{}
		, bIsMonitoring{ false }
		, ThreadID{ -1 }
	{}

	FileMonitor::~FileMonitor()
	{
		StopMonitor();
	}

	void FileMonitor::StopMonitor()
	{
		bIsMonitoring = false;

		if (ThreadID != -1)
		{
			Threading::ThreadManager::GetInstance().WaitOnThread(ThreadID);

			ThreadID = -1;
		}
	}

	void FileMonitor::StartMonitoringFile(const std::string& Filepath)
	{
		{
			ACQUIRE_LOCK();

			if (Filepaths.Contains([&Filepath](const TPair<std::string, int64>& Pair)->bool
				{
					return Filepath == Pair.Key();
				}))
			{
				return;
			}
		}

		{
			ACQUIRE_LOCK();

			const int64 LastTimeModified{ GetLastTimeModified(Filepath.c_str()) };

			if (!LastTimeModified)
			{
				LOG(FileMonitorLog, LogErrorLevel::Error, "Could not retrieve LastTimeModified for file {}. Did not start monitoring file!", Filepath);
				return;
			}

			Filepaths.Add(MakePair(Filepath, LastTimeModified));
		}

		bIsMonitoring = true;

		if (ThreadID == -1)
		{
			ThreadID = Threading::ThreadManager::ScheduleTask([this]()->void
				{
					while (bIsMonitoring)
					{
						constexpr static int32 NrOfMSToWait{ 33 };
						std::this_thread::sleep_for(std::chrono::milliseconds(NrOfMSToWait));

						ACQUIRE_LOCK();

						for (auto& [Filepath, LastTimeModified] : Filepaths)
						{
							const int64 LastModified = GetLastTimeModified(Filepath.c_str());

							if (LastModified > LastTimeModified)
							{
								OnFileChanged.Invoke(Filepath);
								LastTimeModified = LastModified;
							}
						}
					}
				});
		}
	}

	void FileMonitor::StopMonitoringFile(const std::string& Filepath)
	{
		ACQUIRE_LOCK();

		Filepaths.Erase([&Filepath](const TPair<std::string, int64>& Pair)->bool
			{
				return Filepath == Pair.Key();
			});
	}

	void FileMonitor::BindToOnFileChanged(const std::function<void(std::string)>& Callback)
	{
		ACQUIRE_LOCK();

		OnFileChanged.Bind(Callback);
	}
}