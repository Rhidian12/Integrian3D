#pragma once

#include <memory> /* std::unique_ptr */
#include <array>

#include "KeyboardInput/KeyboardInput.h"

namespace Integrian3D
{
	class InputManager final
	{
	public:
		static InputManager& GetInstance();

		void ProcessInput();

		bool GetIsKeyPressed(const KeyboardInput input) const;

	private:
		InputManager();
		void LogInputErrorMessage();

		friend std::unique_ptr<InputManager> std::make_unique<InputManager>();
		inline static std::unique_ptr<InputManager> Instance{};

		std::array<PBYTE, 256> PreviousKeyStates;
		std::array<PBYTE, 256> CurrentKeyStates;
	};
}