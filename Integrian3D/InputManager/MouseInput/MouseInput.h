#pragma once

#include "../../EngineConstants.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h> /* Virtual Keys */
#endif

namespace Integrian3D
{
#ifdef _WIN32
	enum class MouseInput : int
	{
		LMB			= VK_LBUTTON,
		MMB			= VK_MBUTTON,
		RMB			= VK_RBUTTON,
	};
#elif
#error There is no support for platforms outside Windows!
#endif
}