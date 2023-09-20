#include "SceneManager.h"
#include "../DebugUtility/DebugUtility.h"

#include <algorithm> /* std::find_if */

namespace Integrian3D
{
	SceneManager::SceneManager()
		: m_Scenes{}
		, m_pActiveScene{}
	{}

	SceneManager& SceneManager::GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = std::make_unique<SceneManager>();

		return *m_pInstance.get();
	}

	void SceneManager::AddScene(Scene* pScene)
	{
		m_Scenes.push_back(pScene);

		if (!m_pActiveScene)
			m_pActiveScene = pScene;
	}

	void SceneManager::ChangeScene(const std::string_view sceneName)
	{
		const auto cIt{ std::find_if(m_Scenes.cbegin(), m_Scenes.cend(), [sceneName](const Scene* const pScene)
			{
				return pScene->GetSceneName() == sceneName;
			}) };

		__ASSERT(cIt != m_Scenes.cend(), "SceneManager::ChangeScene() > No scene with the specified name %s was found", sceneName);

		// m_pActiveScene->OnSceneLeave();

		m_pActiveScene = *cIt;

		// m_pActiveScene->OnSceneEnter();
	}

	Scene* const SceneManager::GetScene(const std::string_view sceneName)
	{
		const auto it{ std::find_if(m_Scenes.begin(), m_Scenes.end(), [sceneName](const Scene* const pScene)
			{
				return pScene->GetSceneName() == sceneName;
			}) };

		if (it == m_Scenes.end())
		{
			LOG(SceneManagerLog, LogErrorLevel::Error, "No scene with the name: %s was found", sceneName);
			return nullptr;
		}

		return *it;
	}

	const Scene* const SceneManager::GetScene(const std::string_view sceneName) const
	{
		const auto cIt{ std::find_if(m_Scenes.cbegin(), m_Scenes.cend(), [sceneName](const Scene* const pScene)
			{
				return pScene->GetSceneName() == sceneName;
			}) };

		if (cIt == m_Scenes.cend())
		{
			LOG(SceneManagerLog, LogErrorLevel::Error, "No scene with the name: %s was found", sceneName);
			return nullptr;
		}

		return *cIt;
	}
}