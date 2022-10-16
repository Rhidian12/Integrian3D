#include "InputManager.h"

#include "../Core/Core.h"
#include "../DebugUtility/DebugUtility.h"
#include "../Window/Window.h"

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
		__ASSERT(Instance != nullptr && "InputManager::GetInstance() > No InputManager has been created");

		return *Instance.get();
	}

	void InputManager::ProcessInput()
	{
		glfwPollEvents();

		double x{}, y{};
		glfwGetCursorPos(Core::GetInstance().GetWindow().GetWindow(), &x, &y);

		/* Update MousePosition and PreviousMousePosition */
		SetMousePosition(MathUtils::Vec2D{ x, y });

		//std::cout << "Previous:\t";
		//Debug::LogVector(PreviousMousePosition, Debug::MessageColour::White);
		//std::cout << "Current:\t";
		//Debug::LogVector(MousePosition, Debug::MessageColour::White);
		//std::cout << "----------------------\n";
	}

	bool InputManager::GetIsKeyPressed(const KeyboardInput input) const
	{
		return glfwGetKey(pWindow->GetWindow(), static_cast<int>(input));
	}

	bool InputManager::GetIsMouseButtonPressed(const MouseInput mouseInput) const
	{
		return glfwGetMouseButton(pWindow->GetWindow(), static_cast<int>(mouseInput));
	}

	void InputManager::SetMousePosition(const MathUtils::Vec2D& mousePosition)
	{
		PreviousMousePosition = MousePosition;
		MousePosition = mousePosition;
	}
}