#include "InputManager.h"

namespace Integrian3D
{
	InputManager::InputManager()
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
	}
}