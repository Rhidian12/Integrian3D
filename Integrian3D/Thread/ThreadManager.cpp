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

		#define ACQUIRE_LOCK() const std::unique_lock<std::mutex> Lock{ TaskMutex }
		#define SCOPED_LOCK_WITH_EXPR(Expr)	{ \
												const std::unique_lock<std::mutex> Lock{ TaskMutex }; \
												Expr; \
											}

		static void ThreadTask_Internal(TArray<Detail::ThreadTask>& Tasks, bool& bShouldRun)
		{
			using namespace Detail;

			while (true)
			{
				ThreadTask Task{};
				{
					// although we could use ACQUIRE_LOCK(), We don't to keep the readability of the var name here since we need it
					// for the condition variable
					std::unique_lock<std::mutex> Lock{ TaskMutex };

					ConditionVar.wait_for(Lock, std::chrono::milliseconds(33), [&Tasks, bShouldRun]()->bool
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

					LOG(ThreadingLog, LogErrorLevel::Log, "Finishing Task with ID {}", Task.TaskIndex);

					Delegate<int32>& OnTaskCompleted = ThreadManager::GetInstance().GetOnTaskCompletedDelegate();
					SCOPED_LOCK_WITH_EXPR(OnTaskCompleted.Invoke(Task.TaskIndex));

					SCOPED_LOCK_WITH_EXPR(Tasks.Erase(Task));
				}
			}
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
			Threads.EmplaceBack(ThreadTask_Internal, std::ref(Tasks), std::ref(bShouldRun));
		}
	}

	ThreadManager::~ThreadManager()
	{
		bShouldRun = false;

		ConditionVar.notify_all();

		for (auto& thread : Threads)
		{
			thread.join();
		}
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
		ACQUIRE_LOCK();

		const int32 TaskIndex{ NextTaskIndex++ };
		Tasks.EmplaceBack(Task, TaskIndex, false);
		ConditionVar.notify_one();

		return TaskIndex;
	}

	Delegate<int32>& ThreadManager::GetOnTaskCompletedDelegate()
	{
		ACQUIRE_LOCK();

		return OnTaskCompleted;
	}

	void ThreadManager::WaitOnThread(const int32 TaskID)
	{
		TArray<Detail::ThreadTask>::It It{ nullptr };

		{
			ACQUIRE_LOCK();
			It = Tasks.end();
		}

		while (true)
		{
			{
				ACQUIRE_LOCK();

				It = Tasks.Find([TaskID](const Detail::ThreadTask& TTask)->bool
					{
						return TTask.TaskIndex == TaskID;
					});

				if (It == Tasks.end())
				{
					break;
				}
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(15));
		}
	}

	void ThreadManager::StopAllThreads()
	{
		ACQUIRE_LOCK();

		bShouldRun = false;
		ConditionVar.notify_all();
	}
}