#include "SceneManager.h"

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
}