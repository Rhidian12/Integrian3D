#pragma once

#include "TypeDefinitions.h"

namespace Integrian3D
{
	inline volatile bool g_IsRunning{ true };
	inline volatile int32 GMainThreadID{ -1 };
}