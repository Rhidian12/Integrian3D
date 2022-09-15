#include "SceneManager.h"

#include <algorithm> /* std::find_if */

namespace Integrian3D
{
	SceneManager::SceneManager()
		: Scenes{}
		, pActiveScene{}
	{}

	SceneManager& SceneManager::GetInstance()
	{
		if (!Instance)
		{
			Instance = std::make_unique<SceneManager>();
		}

		return *Instance.get();
	}

	void SceneManager::AddScene(Scene&& scene)
	{
		Scenes.push_back(std::move(scene));

		if (!pActiveScene)
		{
			pActiveScene = &Scenes.back();
		}
	}

	void SceneManager::ChangeScene(const std::string_view sceneName)
	{
		auto it{ std::find_if(Scenes.begin(), Scenes.end(), [sceneName](const Scene& scene)
			{
				return scene.GetSceneName() == sceneName;
			}) };

		__ASSERT(it != Scenes.end() && "SceneManager::ChangeScene() > No scene with the specified name was found");

		pActiveScene->OnSceneLeave();

		pActiveScene = &(*it);

		pActiveScene->OnSceneEnter();
	}

	Scene& SceneManager::GetScene(const std::string_view sceneName)
	{
		const auto it{ std::find_if(Scenes.begin(), Scenes.end(), [sceneName](const Scene& scene)
			{
				return scene.GetSceneName() == sceneName;
			}) };

		__ASSERT(it != Scenes.end() && "SceneManager::GetScene() > No scene with the specified name was found");

		return *it;
	}

	const Scene& SceneManager::GetScene(const std::string_view sceneName) const
	{
		const auto cIt{ std::find_if(Scenes.cbegin(), Scenes.cend(), [sceneName](const Scene& scene)
			{
				return scene.GetSceneName() == sceneName;
			}) };

		__ASSERT(cIt != Scenes.end() && "SceneManager::GetScene() > No scene with the specified name was found");

		return *cIt;
	}
}