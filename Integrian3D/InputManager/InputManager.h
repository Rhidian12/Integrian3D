#pragma once

#include <memory> /* std::unique_ptr */

namespace Integrian3D
{
	class InputManager final
	{
	public:
		static InputManager& GetInstance();

		void ProcessInput();

	private:
		InputManager();

		friend std::unique_ptr<InputManager> std::make_unique<InputManager>();
		inline static std::unique_ptr<InputManager> Instance{};
	};
}