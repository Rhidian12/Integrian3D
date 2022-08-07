#include "Scene.h"

namespace Integrian3D
{
	Scene::Scene(const std::string& sceneName)
		: InitializeCallback{}
		, Registry{}
		, SceneName{ sceneName }
	{
	}

	void Scene::Start()
	{
		InitializeCallback(*this);
	}

	Entity Scene::CreateEntity()
	{
		const Entity entity{ Registry.CreateEntity() };

		/* [TODO]: Add Transform Component */

		return entity;
	}
}