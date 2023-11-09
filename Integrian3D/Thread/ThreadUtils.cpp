#include "Thread/ThreadUtils.h"

#include <thread>

namespace Integrian3D
{
	uint32 GetThreadID()
	{
		return static_cast<uint32>(std::hash<std::thread::id>{}(std::this_thread::get_id()));
	}

	bool IsMainThread()
	{
		return GetThreadID() == GMainThreadID;
	}
}