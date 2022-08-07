#include "Scene.h"

namespace Integrian3D
{
	Scene::Scene()
		: InitializeCallback{}
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