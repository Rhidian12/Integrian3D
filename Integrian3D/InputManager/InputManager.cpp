#include "InputManager.h"

#include "Core/Core.h"
#include "Window/Window.h"

namespace Integrian3D
{
	/* Defined in Core.h */
	extern inline volatile bool g_IsRunning;

	InputManager::InputManager(Detail::Window* pWindow)
		: PreviousMousePosition{}
		, MousePosition{}
		, pWindow{ pWindow }
	{}

	void InputManager::CreateInputManager(Detail::Window* pWindow)
	{
		if (!Instance)
		{
			Instance.reset(new InputManager{ pWindow });
		}
	}

	InputManager& InputManager::GetInstance()
	{
		__CHECK(Instance != nullptr);

		return *Instance.get();
	}

	void InputManager::ProcessInput()
	{
		glfwPollEvents();

		double x{}, y{};
		glfwGetCursorPos(Core::GetInstance().GetWindow().GetWindow(), &x, &y);

		/* Update MousePosition and PreviousMousePosition */
		SetMousePosition(Math::Vec2D{ x, y });
	}

	bool InputManager::GetIsKeyPressed(const KeyboardInput input) const
	{
		return glfwGetKey(pWindow->GetWindow(), static_cast<int>(input));
	}

	bool InputManager::GetIsMouseButtonPressed(const MouseInput mouseInput) const
	{
		return glfwGetMouseButton(pWindow->GetWindow(), static_cast<int>(mouseInput));
	}

	void InputManager::SetMousePosition(const Math::Vec2D& mousePosition)
	{
		PreviousMousePosition = MousePosition;
		MousePosition = mousePosition;
	}
}