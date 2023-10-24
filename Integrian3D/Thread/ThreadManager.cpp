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

		static void ThreadTask_Internal(TArray<Detail::ThreadTask>& Tasks, bool& bShouldRun, int32 ID)
		{
			using namespace Detail;

			while (true)
			{
				ThreadTask Task{};
				{
					std::unique_lock<std::mutex> Lock{ TaskMutex };

					LOG(ThreadingLog, LogErrorLevel::Log, "Thread with ID {} waiting!", ID);

					ConditionVar.wait(Lock, [&Tasks, bShouldRun]()->bool
						{
							return !bShouldRun || !Tasks.Empty();
						});

					// Check if the program has ended
					if (!bShouldRun)
					{
						break;
					}

					for (Detail::ThreadTask& ThreadTask : Tasks)
					{
						if (!ThreadTask.bInProgress)
						{
							Task = ThreadTask;
							ThreadTask.bInProgress = true; // although not entirely true yet, this prevents another thread from taking the task as well
						}
					}
				}

				if (Task.Task)
				{
					LOG(ThreadingLog, LogErrorLevel::Log, "Starting Task with ID {}", Task.TaskIndex);

					Task.Task();
					Task.bIsCompleted = true;

					const std::unique_lock<std::mutex> Lock{ TaskMutex };

					LOG(ThreadingLog, LogErrorLevel::Log, "Finishing Task with ID {}", Task.TaskIndex);

					ThreadManager::GetInstance().GetOnTaskCompletedDelegate().Invoke(Task.TaskIndex);

					Tasks.Erase([Task](const Detail::ThreadTask& TTask)->bool
						{
							return TTask.TaskIndex == Task.TaskIndex;
						});
				}
			}

			LOG(ThreadingLog, LogErrorLevel::Log, "Thread {} finishing", ID);
		}
	}

	ThreadManager::ThreadManager()
		: MaxNrOfThreads{}
		, bShouldRun{ true }
		, NextTaskIndex{}
	{
		const IniFile EngineIni{ "Config/Engine.ini" };
		EngineIni.GetInteger("Threading", "MaxNrOfThreads", MaxNrOfThreads);

		if (MaxNrOfThreads > static_cast<int32>(std::thread::hardware_concurrency()))
		{
			MaxNrOfThreads = std::thread::hardware_concurrency();
		}

		for (int32 i{}; i < MaxNrOfThreads; ++i)
		{
			Threads.EmplaceBack(ThreadTask_Internal, std::ref(Tasks), std::ref(bShouldRun), i);
		}
	}

	ThreadManager::~ThreadManager()
	{
		bShouldRun = false;

		ConditionVar.notify_all();
	}

	ThreadManager& ThreadManager::GetInstance()
	{
		static ThreadManager Instance{};
		return Instance;
	}

	int32 ThreadManager::ScheduleTask(const std::function<void()>& Task)
	{
		ThreadManager& Manager = ThreadManager::GetInstance();

		return Manager.ScheduleTask_Impl(Task);
	}

	int32 ThreadManager::ScheduleTask_Impl(const std::function<void()>& Task)
	{
		std::unique_lock<std::mutex> Lock{ TaskMutex };

		const int32 TaskIndex{ NextTaskIndex++ };
		Tasks.EmplaceBack(Task, TaskIndex, false, false);
		ConditionVar.notify_one();

		return TaskIndex;
	}

	Delegate<int32>& ThreadManager::GetOnTaskCompletedDelegate()
	{
		const std::unique_lock<std::mutex> Lock{ TaskMutex };

		return OnTaskCompleted;
	}

	void ThreadManager::WaitOnThread(const int32 TaskID)
	{
		Iterator It{ Tasks.end() };

		do
		{
			{
				const std::unique_lock<std::mutex> Lock{ TaskMutex };

				It = Tasks.Find([TaskID](const Detail::ThreadTask& TTask)->bool
					{
						return TTask.TaskIndex == TaskID;
					});
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(15));

		} while (It != Tasks.end());
	}

	void ThreadManager::StopAllThreads()
	{
		const std::unique_lock<std::mutex> Lock{ TaskMutex };

		bShouldRun = false;
		ConditionVar.notify_all();
	}
}