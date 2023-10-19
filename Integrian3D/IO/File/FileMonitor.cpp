#include "IO/File/FileMonitor.h"

#include "IO/PathUtils.h"
#include "Thread/ThreadManager.h"
#include "Timer/TimeLength.h"
#include "Win32Utils/Win32APICall.h"

I_DISABLE_WARNING(4005) // warning C4005: 'APIENTRY': macro redefinition
#include <Windows.h>
I_ENABLE_WARNING(4005)

namespace Integrian3D::IO
{
	namespace
	{
		static bool CloseChangeNotificationHandle(void* Handle)
		{
			return CALL_WIN32_RV_IGNORE_ERROR(FindCloseChangeNotification(Handle), TRUE);
		}
	}

	FileMonitor::FileMonitor(const std::string& Path)
		: Handle{}
		, Filepath{ Path }
		, bIsMonitoring{ false }
	{}

	void FileMonitor::StartMonitoringFile()
	{
		Handle = Win32Utils::Win32Handle
		{ 
			CALL_WIN32_RV
			(
				FindFirstChangeNotificationA(PathUtils::GetPathWithoutExtension(Filepath).data(), false, FILE_NOTIFY_CHANGE_SIZE)
			),
			CloseChangeNotificationHandle
		};

		bIsMonitoring = true;

		Threading::ThreadManager::GetInstance().ScheduleTask([this]()->void
			{
				while (bIsMonitoring)
				{
					constexpr static int32 WaitTimeSec{ 1 };
					const DWORD WaitStatus{ CALL_WIN32_RV_IGNORE_ERRORS(WaitForSingleObject(Handle, static_cast<DWORD>(WaitTimeSec * Time::SecToMilli)),
						WAIT_OBJECT_0, WAIT_TIMEOUT) };

					if (WaitStatus == WAIT_OBJECT_0)
					{
						OnFileChanged.Invoke(Filepath);
						CALL_WIN32_IGNORE_ERROR(FindNextChangeNotification(Handle), TRUE);
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