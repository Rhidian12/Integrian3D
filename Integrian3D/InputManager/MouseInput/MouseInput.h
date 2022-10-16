#pragma once

#include "../../EngineConstants.h"

namespace Integrian3D
{
	enum class MouseInput : int
	{
		LMB			= GLFW_MOUSE_BUTTON_LEFT,
		MMB			= GLFW_MOUSE_BUTTON_MIDDLE,
		RMB			= GLFW_MOUSE_BUTTON_RIGHT,
	};
}