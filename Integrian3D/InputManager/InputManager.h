#pragma once

#include "../EngineConstants.h"
#include "KeyboardInput/KeyboardInput.h"
#include "MouseInput/MouseInput.h"
#include "../Math/MathUtils.h"

#include <memory> /* std::unique_ptr */

namespace Integrian3D
{
	namespace Detail
	{
		class Window;
	}

	class InputManager final
	{
	public:
		static void CreateInputManager(Detail::Window* pWindow);
		static InputManager& GetInstance();

		void ProcessInput();

		__NODISCARD bool GetIsKeyPressed(const KeyboardInput input) const;
		__NODISCARD bool GetIsMouseButtonPressed(const MouseInput mouseInput) const;

		void SetMousePosition(const MathUtils::Vec2D& mousePos);

		__NODISCARD const MathUtils::Vec2D& GetMousePosition() const { return MousePosition; }
		__NODISCARD const MathUtils::Vec2D& GetPreviousMousePosition() const { return PreviousMousePosition; }

	private:
		InputManager(Detail::Window* pWindow);

		inline static std::unique_ptr<InputManager> Instance{};

		MathUtils::Vec2D PreviousMousePosition;
		MathUtils::Vec2D MousePosition;

		Detail::Window* pWindow;
	};
}