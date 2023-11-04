#include "Thread/ThreadUtils.h"

#include <thread>

namespace Integrian3D
{
	int32 GetThreadID()
	{
		return static_cast<int32>(std::hash<std::thread::id>{}(std::this_thread::get_id()));
	}

	bool IsMainThread()
	{
		return GetThreadID() == GMainThreadID;
	}
}