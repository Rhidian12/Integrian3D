#include "Thread/ThreadManager.h"

#include "Core/Core.h"
#include "Delegate/Delegate.h"
#include "IO/Ini/IniFile.h"

#include <mutex>

namespace Integrian3D::Threading
{
	namespace
	{
		static std::mutex TaskMutex;
		static std::condition_variable ConditionVar;

		static void ThreadTask_Internal(TArray<Detail::ThreadTask>& Tasks)
		{
			using namespace Detail;

			while (true)
			{
				ThreadTask Task{};
				{
					std::unique_lock<std::mutex> Lock{ TaskMutex };

					ConditionVar.wait(Lock, [&Tasks]()->bool
						{
							return !g_IsRunning || !Tasks.Empty();
						});

					// Check if the program has ended
					if (!g_IsRunning)
					{
						return;
					}

					if (!Tasks.Empty())
					{
						Task = Tasks[0];
						Tasks.PopFront();
					}
				}

				if (Task.Task)
				{
					Task.Task();
				}

				{
					const std::unique_lock<std::mutex> Lock{ TaskMutex };

					ThreadManager::GetInstance().GetOnTaskCompletedDelegate().Invoke(Task.TaskID);
				}
			}
		}
	}

	ThreadManager::ThreadManager()
		: MaxNrOfThreads{}
	{
		const IniFile EngineIni{ "Config/Engine.ini" };
		EngineIni.GetInteger("Threading", "MaxNrOfThreads", MaxNrOfThreads);

		if (MaxNrOfThreads > static_cast<int32>(std::thread::hardware_concurrency()))
		{
			MaxNrOfThreads = std::thread::hardware_concurrency();
		}

		for (int32 i{}; i < MaxNrOfThreads; ++i)
		{
			Threads.EmplaceBack(ThreadTask_Internal, std::ref(Tasks));
		}
	}

	ThreadManager::~ThreadManager()
	{
		ConditionVar.notify_all();
	}

	ThreadManager& ThreadManager::GetInstance()
	{
		static ThreadManager Instance{};
		return Instance;
	}

	int32 ThreadManager::ScheduleTask(const std::function<void()>& Task)
	{
		const int32 TaskID{ NextTaskID++ };
		Tasks.EmplaceBack(Task, TaskID);
		return TaskID;
	}

	Delegate<int32>& ThreadManager::GetOnTaskCompletedDelegate()
	{
		const std::unique_lock<std::mutex> Lock{ TaskMutex };

		return OnTaskCompleted;
	}
}