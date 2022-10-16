#pragma once

#include "../EngineConstants.h"
#include "KeyboardInput/KeyboardInput.h"
#include "MouseInput/MouseInput.h"
#include "../Math/MathUtils.h"

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

		void SetMousePosition(const MathUtils::Vec2D& mousePos);

		// __NODISCARD const MathUtils::Vec2D& GetMousePosition() const { return MousePosition; }
		__NODISCARD MathUtils::Vec2D GetMousePosition() const;
		__NODISCARD const MathUtils::Vec2D& GetPreviousMousePosition() const { return PreviousMousePosition; }

	private:
		InputManager();
		void LogInputErrorMessage();

		friend std::unique_ptr<InputManager> std::make_unique<InputManager>();
		inline static std::unique_ptr<InputManager> Instance{};

		std::array<PBYTE, 256> PreviousKeyStates;
		std::array<PBYTE, 256> CurrentKeyStates;

		MathUtils::Vec2D PreviousMousePosition;
		MathUtils::Vec2D MousePosition;
	};
}