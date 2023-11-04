#include "InputManager.h"

#include "Core/Core.h"
#include "Window/Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Integrian3D
{
	InputManager::InputManager()
		: PreviousMousePosition{}
		, MousePosition{}
	{}

	InputManager& InputManager::GetInstance()
	{
		static InputManager Manager{};

		return Manager;;
	}

	void InputManager::ProcessInput()
	{
		glfwPollEvents();

		double x{}, y{};
		glfwGetCursorPos(GWindow->GetWindow(), &x, &y);

		/* Update MousePosition and PreviousMousePosition */
		SetMousePosition(Math::Vec2D{ x, y });
	}

	bool InputManager::GetIsKeyPressed(const KeyboardInput input) const
	{
		return glfwGetKey(GWindow->GetWindow(), static_cast<int>(input));
	}

	bool InputManager::GetIsMouseButtonPressed(const MouseInput mouseInput) const
	{
		return glfwGetMouseButton(GWindow->GetWindow(), static_cast<int>(mouseInput));
	}

	void InputManager::SetMousePosition(const Math::Vec2D& mousePosition)
	{
		PreviousMousePosition = MousePosition;
		MousePosition = mousePosition;
	}
}