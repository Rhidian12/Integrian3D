#include "SceneManager.h"

#include <algorithm> /* std::find_if */

namespace Integrian3D
{
	SceneManager::SceneManager()
		: Scenes{}
	{}

	SceneManager& SceneManager::GetInstance()
	{
		if (!Instance)
		{
			Instance = std::make_unique<SceneManager>();
		}

		return *Instance.get();
	}

	Scene& SceneManager::GetScene(const std::string_view sceneName)
	{
		const auto it{ std::find_if(Scenes.begin(), Scenes.end(), [sceneName](const Scene& scene)
			{
				return scene.GetSceneName() == sceneName;
			}) };

		assert(it != Scenes.end());

		return *it;
	}

	const Scene& SceneManager::GetScene(const std::string_view sceneName) const
	{
		const auto cIt{ std::find_if(Scenes.cbegin(), Scenes.cend(), [sceneName](const Scene& scene)
			{
				return scene.GetSceneName() == sceneName;
			}) };

		assert(cIt != Scenes.cend());

		return *cIt;
	}
}