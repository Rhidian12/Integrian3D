#pragma once

#include "../EngineConstants.h"
#include "KeyboardInput/KeyboardInput.h"
#include "MouseInput/MouseInput.h"
#include "../Math/Math.h"

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
		__NODISCARD static InputManager& GetInstance();

		void ProcessInput();

		__NODISCARD bool GetIsKeyPressed(const KeyboardInput input) const;
		__NODISCARD bool GetIsMouseButtonPressed(const MouseInput mouseInput) const;

		void SetMousePosition(const Math::Vec2D& mousePos);

		__NODISCARD const Math::Vec2D& GetMousePosition() const { return MousePosition; }
		__NODISCARD const Math::Vec2D& GetPreviousMousePosition() const { return PreviousMousePosition; }

	private:
		InputManager(Detail::Window* pWindow);

		inline static std::unique_ptr<InputManager> Instance{};

		Math::Vec2D PreviousMousePosition;
		Math::Vec2D MousePosition;

		Detail::Window* pWindow;
	};
}