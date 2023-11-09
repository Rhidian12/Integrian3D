#pragma once

#include "TypeDefinitions.h"

namespace Integrian3D
{
	inline bool g_IsRunning{ true };
	inline uint32 GMainThreadID{ 0 };
	inline class Window* GWindow{}; // Managed by Core
}