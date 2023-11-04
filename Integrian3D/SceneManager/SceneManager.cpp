#include "SceneManager.h"

#include <algorithm> /* std::find_if */

namespace Integrian3D
{
	SceneManager::SceneManager()
		: Scenes{}
		, ActiveScene{}
	{}

	SceneManager& SceneManager::GetInstance()
	{
		static SceneManager Instance{};

		return Instance;
	}

	void SceneManager::ChangeScene(const std::string_view sceneName)
	{
		const auto It{ Scenes.Find([sceneName](const UniquePtr<Scene>& Scene)
			{
				return Scene->GetSceneName() == sceneName;
			}) };

		__ASSERT(It != Scenes.end(), "SceneManager::ChangeScene() > No scene with the specified name {} was found", sceneName);

		// m_pActiveScene->OnSceneLeave();

		ActiveScene = It->Get();

		// m_pActiveScene->OnSceneEnter();
	}

	Scene* const SceneManager::GetActiveScene()
	{
		return ActiveScene;
	}

	const Scene* const SceneManager::GetActiveScene() const
	{
		return ActiveScene;
	}

	Scene* const SceneManager::GetScene(const std::string_view sceneName)
	{
		// [CRINGE]: stop using the same lambda everywhere
		const auto It{ Scenes.Find([sceneName](const UniquePtr<Scene>& Scene)
			{
				return Scene->GetSceneName() == sceneName;
			}) };

		if (It == Scenes.end())
		{
			LOG(SceneManagerLog, LogErrorLevel::Error, "No scene with the name: %s was found", sceneName);
			return nullptr;
		}

		return It->Get();
	}

	const Scene* const SceneManager::GetScene(const std::string_view sceneName) const
	{
		const auto It{ Scenes.Find([sceneName](const UniquePtr<Scene>& Scene)
			{
				return Scene->GetSceneName() == sceneName;
			}) };

		if (It == Scenes.cend())
		{
			LOG(SceneManagerLog, LogErrorLevel::Error, "No scene with the name: %s was found", sceneName);
			return nullptr;
		}

		return It->Get();
	}

	const TArray<UniquePtr<Scene>>& SceneManager::GetAllScenes() const
	{
		return Scenes;
	}

	void SceneManager::ClearAllScenes()
	{
		Scenes.Clear();
		ActiveScene = nullptr;
	}

	void SceneManager::InitializeAllScenes()
	{
		for (UniquePtr<Scene>& Scene : Scenes)
		{
			Scene->Start();
		}
	}
}