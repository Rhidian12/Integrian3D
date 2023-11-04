#pragma once

#include "EngineConstants.h"

#include "Array/Array.h"
#include "Memory/UniquePtr.h"

#include "Scene/Scene.h"

#include <string_view> /* std::string_view */

DEFINE_LOG_CATEGORY(SceneManagerLog, Integrian3D::LogVerbosity::Verbose);

namespace Integrian3D
{
	class SceneManager final
	{
	public:
		static SceneManager& GetInstance();

		template<typename T>
		__NODISCARD Scene* AddScene(const std::string& SceneName);

		void ChangeScene(const std::string_view sceneName);

		__NODISCARD Scene* const GetActiveScene();
		__NODISCARD const Scene* const GetActiveScene() const;

		__NODISCARD Scene* const GetScene(const std::string_view sceneName);
		__NODISCARD const Scene* const GetScene(const std::string_view sceneName) const;

		/* [TODO]: Write wrappers instead of allowing access to all scenes */
		__NODISCARD const TArray<UniquePtr<Scene>>& GetAllScenes() const;

	private:
		friend class Core;
		void ClearAllScenes();
		void InitializeAllScenes();

		SceneManager();

		TArray<UniquePtr<Scene>> Scenes;
		Scene* ActiveScene;
	};

	template<typename T>
	Scene* SceneManager::AddScene(const std::string& SceneName)
	{
		Scenes.Add(MakeUnique<T>(SceneName));

		if (!ActiveScene)
		{
			ActiveScene = Scenes.Back().Get();
		}

		return Scenes.Back().Get();
	}
}