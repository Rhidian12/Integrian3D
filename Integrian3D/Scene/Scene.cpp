#include "Scene.h"

#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/CameraComponent/CameraComponent.h"
#include "../Math/Math.h"
#include "../Core/Core.h"

#include <gtc/matrix_transform.hpp>

namespace Integrian3D
{
	Scene::Scene(const std::string& sceneName)
		: m_SceneName{ sceneName }
		, m_GameObjects{}
		, m_pActiveCamera{}
	{}

	Scene::~Scene()
	{
		for (const GameObject* pG : m_GameObjects)
		{
			__DELETE(pG);
		}
	}

	void Scene::AddGameObject(GameObject* const pGameobject)
	{
		if (m_GameObjects.Find(pGameobject) == m_GameObjects.cend())
			m_GameObjects.Add(pGameobject);
	}

	void Scene::Start()
	{
		for (GameObject* pG : m_GameObjects)
		{
			pG->Start();

			if (CameraComponent* pC{ pG->GetComponentByType<CameraComponent>() }; pC != nullptr)
				m_pActiveCamera = pC;
		}

		if (!m_pActiveCamera)
		{
			GameObject* pCamera{ Instantiate("MainCamera", this) };
			m_pActiveCamera = pCamera->AddComponent(new CameraComponent
				{
					pCamera,
					0.1f,
					100.f,
					Math::ToRadians(45.f),
					static_cast<float>(Core::GetInstance().GetWindowWidth()) / Core::GetInstance().GetWindowHeight()
				});

			pCamera->pTransform->Translate(Math::Vec3D{ 0.f, 0.f, -10.f }, true);

			pCamera->Start();

			AddGameObject(pCamera);
		}
	}

	void Scene::Update()
	{
		for (GameObject* pG : m_GameObjects)
			pG->Update();
	}

	void Scene::Render() const
	{
		for (const GameObject* pG : m_GameObjects)
			pG->Render();
	}
}