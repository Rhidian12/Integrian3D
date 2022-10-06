#pragma once

#include "../EngineConstants.h"
#include "KeyboardInput/KeyboardInput.h"
#include "MouseInput/MouseInput.h"

#include <memory> /* std::unique_ptr */
#include <array> /* std::array */

namespace Integrian3D
{
	class InputManager final
	{
	public:
		static InputManager& GetInstance();

		void ProcessInput();

		__NODISCARD bool GetIsKeyPressed(const KeyboardInput input) const;
		__NODISCARD bool GetIsMouseButtonPressed(const MouseInput mouseInput) const;

	private:
		InputManager();
		void LogInputErrorMessage();

		friend std::unique_ptr<InputManager> std::make_unique<InputManager>();
		inline static std::unique_ptr<InputManager> Instance{};

		std::array<PBYTE, 256> PreviousKeyStates;
		std::array<PBYTE, 256> CurrentKeyStates;
	};
}