#pragma once

#include "../EngineConstants.h"
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

		void AddScene(Scene&& scene);

		void ChangeScene(const std::string_view sceneName);

		__NODISCARD Scene& GetActiveScene() { assert(pActiveScene); return *pActiveScene; }
		__NODISCARD const Scene& GetActiveScene() const { assert(pActiveScene); return *pActiveScene; }

		__NODISCARD Scene& GetScene(const std::string_view sceneName);
		__NODISCARD const Scene& GetScene(const std::string_view sceneName) const;

		__NODISCARD const std::vector<Scene>& GetAllScenes() const { return Scenes; }

	private:
		SceneManager();

		friend std::unique_ptr<SceneManager> std::make_unique();
		inline static std::unique_ptr<SceneManager> Instance{};

		std::vector<Scene> Scenes;
		Scene* pActiveScene;
	};
}