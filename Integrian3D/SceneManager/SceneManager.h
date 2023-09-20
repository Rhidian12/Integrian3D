#pragma once

#include "EngineConstants.h"
#include "Scene/Scene.h"

#include <memory> /* std::unique_ptr */
#include <vector> /* std::vector */
#include <string_view> /* std::string_view */

DEFINE_LOG_CATEGORY(SceneManagerLog, Integrian3D::LogVerbosity::Verbose);

namespace Integrian3D
{
	class SceneManager final
	{
	public:
		static SceneManager& GetInstance();

		void AddScene(Scene* scene);

		void ChangeScene(const std::string_view sceneName);

		__NODISCARD Scene* const GetActiveScene() { return m_pActiveScene; }
		__NODISCARD const Scene* const GetActiveScene() const { return m_pActiveScene; }

		__NODISCARD Scene* const GetScene(const std::string_view sceneName);
		__NODISCARD const Scene* const GetScene(const std::string_view sceneName) const;

		/* [TODO]: Write wrappers instead of allowing access to all scenes */
		__NODISCARD const std::vector<Scene*>& GetAllScenes() const { return m_Scenes; }

	private:
		SceneManager();

		friend std::unique_ptr<SceneManager> std::make_unique();
		inline static std::unique_ptr<SceneManager> m_pInstance{};

		std::vector<Scene*> m_Scenes;
		Scene* m_pActiveScene;
	};
}