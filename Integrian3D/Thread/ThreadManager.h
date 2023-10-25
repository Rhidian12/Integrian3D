#pragma once

#include "EngineConstants.h"
#include "Array/Array.h"
#include "Delegate/Delegate.h"

#include <functional>
#include <thread>
#include <vector>

DEFINE_LOG_CATEGORY(ThreadingLog, Integrian3D::LogVerbosity::Verbose);

namespace Integrian3D::Threading
{
	namespace Detail
	{
		struct ThreadTask final
		{
			std::function<void()> Task;
			int32 TaskIndex;
			bool bInProgress;

			bool operator==(const ThreadTask& Other) const
			{
				return TaskIndex == Other.TaskIndex;
			}
		};
	}

	class ThreadManager
	{
	public:
		~ThreadManager();
		static ThreadManager& GetInstance();

		static int32 ScheduleTask(const std::function<void()>& Task);
		
		Delegate<int32>& GetOnTaskCompletedDelegate();

		void WaitOnThread(const int32 TaskID);

		void StopAllThreads();

	private:
		ThreadManager();
		int32 ScheduleTask_Impl(const std::function<void()>& Task);

		TArray<std::jthread> Threads;
		TArray<Detail::ThreadTask> Tasks;
		Delegate<int32> OnTaskCompleted;

		int32 NextTaskIndex;
		int32 MaxNrOfThreads;
		bool bShouldRun;
	};
}