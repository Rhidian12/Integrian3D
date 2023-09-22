#include "InputManager/InputManager.h"

#include "Core/Core.h"
#include "DebugUtility/DebugUtility.h"
#include "Math/RayToGlm.h"
#include "Window/Window.h"

#include <raylib.h>

namespace Integrian3D
{
	/* Defined in Core.h */
	extern inline volatile bool g_IsRunning;

	InputManager::InputManager()
		: PreviousMousePosition{}
		, MousePosition{}
	{}

	InputManager& InputManager::GetInstance()
	{
		if (!Instance)
		{
			Instance.reset(new InputManager{});
		}

		return *Instance.get();
	}

	void InputManager::ProcessInput()
	{
		SetMousePosition(ToGLM2D(::GetMousePosition()));
	}

	bool InputManager::GetIsKeyPressed(const KeyboardInput input) const
	{
		return IsKeyPressed(static_cast<int>(input));
	}

	bool InputManager::GetIsMouseButtonPressed(const MouseInput mouseInput) const
	{
		return IsMouseButtonPressed(static_cast<int>(mouseInput));
	}

	void InputManager::SetMousePosition(const Math::Vec2D& mousePosition)
	{
		PreviousMousePosition = MousePosition;
		MousePosition = mousePosition;
	}
}