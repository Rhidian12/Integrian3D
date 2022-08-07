#pragma once

#include "../Scene/Scene.h"

#include <memory> /* std::unique_ptr */
#include <vector> /* std::vector */
#include <string_view> /* std::string_view */

namespace Integrian3D
{
	class SceneManager final
	{
	public:
		static SceneManager& GetInstance();

		void AddScene(const Scene& scene) { Scenes.push_back(scene); }

		Scene& GetScene(const std::string_view sceneName);
		const Scene& GetScene(const std::string_view sceneName) const;

	private:
		SceneManager();

		friend std::unique_ptr<SceneManager> std::make_unique();
		inline static std::unique_ptr<SceneManager> Instance{};

		std::vector<Scene> Scenes;
	};
}