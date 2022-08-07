#pragma once

#include <memory> /* std::unique_ptr */

namespace Integrian3D
{
	class SceneManager final
	{
	public:
		static SceneManager& GetInstance();

	private:
		SceneManager();

		friend std::unique_ptr<SceneManager> std::make_unique();
		inline static std::unique_ptr<SceneManager> Instance{};
	};
}