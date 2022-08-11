#include "Scene.h"

#include "../Components/TransformComponent/TransformComponent.h"

namespace Integrian3D
{
	Scene::Scene(const std::string& sceneName)
		: InitializeCallback{}
		, OnSceneEnterCallback{}
		, OnSceneLeaveCallback{}
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

		AddComponent<TransformComponent>(entity);

		return entity;
	}
}