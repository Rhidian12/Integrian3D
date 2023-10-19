#pragma once

#include "EngineConstants.h"
#include "Array/Array.h"
#include "Delegate/Delegate.h"

#include <functional>
#include <thread>

namespace Integrian3D::Threading
{
	namespace Detail
	{
		struct ThreadTask final
		{
			std::function<void()> Task;
			int32 TaskID;
		};
	}

	class ThreadManager
	{
	public:
		~ThreadManager();
		static ThreadManager& GetInstance();

		int32 ScheduleTask(const std::function<void()>& Task);
		
		Delegate<int32>& GetOnTaskCompletedDelegate();

	private:
		ThreadManager();

		TArray<std::jthread> Threads;
		TArray<Detail::ThreadTask> Tasks;
		Delegate<int32> OnTaskCompleted;

		int32 NextTaskID;
		int32 MaxNrOfThreads;
	};
}