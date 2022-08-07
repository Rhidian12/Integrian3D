#pragma once

#include "../Scene/Scene.h"

#include <memory> /* std::unique_ptr */
#include <vector> /* std::vector */

namespace Integrian3D
{
	class SceneManager final
	{
	public:
		static SceneManager& GetInstance();

		void AddScene(const Scene& scene) { Scenes.push_back(scene); }

	private:
		SceneManager();

		friend std::unique_ptr<SceneManager> std::make_unique();
		inline static std::unique_ptr<SceneManager> Instance{};

		std::vector<Scene> Scenes;
	};
}