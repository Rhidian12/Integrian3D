#include "Scene.h"

#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/CameraComponent/CameraComponent.h"
#include "../Math/MathUtils.h"
#include "../Core/Core.h"

namespace Integrian3D
{
	Scene::Scene(const std::string& sceneName)
		: InitializeCallback{}
		, OnSceneEnterCallback{}
		, OnSceneLeaveCallback{}
		, Registry{}
		, SceneName{ sceneName }
		, CameraEntity{ InvalidEntityID }
	{
	}

	void Scene::Start()
	{
		{
			CameraEntity = CreateEntity();
			AddComponent<CameraComponent, float, float, float, float>(
				CameraEntity,
				0.1f,
				100.f,
				MathUtils::ToRadians(45.f),
				static_cast<float>(Core::GetInstance().GetWindowWidth()) / Core::GetInstance().GetWindowHeight());

			CreateView<CameraComponent, TransformComponent>().ForEach([](CameraComponent& camera, const TransformComponent& transform)->void
				{

				});
		}

		if (InitializeCallback)
		{
			InitializeCallback(*this);
		}
	}

	Entity Scene::CreateEntity()
	{
		const Entity entity{ Registry.CreateEntity() };

		AddComponent<TransformComponent>(entity);

		return entity;
	}
}