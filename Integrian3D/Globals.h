#pragma once

#include "TypeDefinitions.h"

namespace Integrian3D
{
	inline bool g_IsRunning{ true };
	inline int32 GMainThreadID{ -1 };
	inline class Window* GWindow{}; // Managed by Core
}