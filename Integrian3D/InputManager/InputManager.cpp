#include "InputManager.h"

#include "../Core/Core.h"
#include "../DebugUtility/DebugUtility.h"

#ifdef _WIN32
#include <WinUser.h> /* Keyboard input */
#include <errhandlingapi.h> /* Error handling */
#include <WinBase.h> /* FormatMessage() */
#endif

#include <GLFW/glfw3.h> /* GLFW */

namespace Integrian3D
{
	/* Defined in Core.h */
	extern inline volatile bool g_IsRunning;

	InputManager::InputManager()
		: PreviousKeyStates{}
		, CurrentKeyStates{}
		, PreviousMousePosition{}
		, MousePosition{}
	{}

	InputManager& InputManager::GetInstance()
	{
		if (!Instance)
		{
			Instance = std::make_unique<InputManager>();
		}

		return *Instance.get();
	}

	void InputManager::ProcessInput()
	{
		//MSG msg;
		//PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
		//TranslateMessage(&msg);
		//DispatchMessage(&msg);

		glfwPollEvents();

		double x{}, y{};
		glfwGetCursorPos(Core::GetInstance().GetWindow().GetWindow(), &x, &y);

		SetMousePosition(MathUtils::Vec2D{ x, y });
	}

	bool InputManager::GetIsKeyPressed(const KeyboardInput input) const
	{
		return GetKeyState(static_cast<int>(input)) & 0x8000;
	}

	bool InputManager::GetIsMouseButtonPressed(const MouseInput mouseInput) const
	{
		return GetKeyState(static_cast<int>(mouseInput)) & 0x8000;
	}

	MathUtils::Vec2D InputManager::GetMousePosition() const
	{
		return MousePosition;
	}

	void InputManager::SetMousePosition(const MathUtils::Vec2D& mousePosition)
	{
		PreviousMousePosition = MousePosition;
		MousePosition = mousePosition;
	}

	void InputManager::LogInputErrorMessage()
	{
		LPVOID messageBuffer;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPTSTR>(&messageBuffer),
			0,
			nullptr);

		Debug::LogError(reinterpret_cast<const char*>(messageBuffer), false);

		LocalFree(messageBuffer);
	}
}