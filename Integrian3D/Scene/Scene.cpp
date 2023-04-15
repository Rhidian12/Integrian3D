#include "Scene.h"

#include "../Math/Math.h"
#include "../Core/Core.h"
#include "../TPair/TPair.full.h"

#include "../Components/CameraComponent/CameraComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"

#include <gtc/matrix_transform.hpp>

namespace Integrian3D
{
	Scene::Scene(const std::string& sceneName)
		: m_SceneName{ sceneName }
		, m_InitCallbacks{}
		, m_Registry{}
	{}

	Scene::~Scene()
	{
		m_Registry.Clear();
	}

	void Scene::AddInitialisationCallback(const size_t prio, void(*fn)(Scene&))
	{
		m_InitCallbacks.Add(MakePair(prio, fn));
	}

	void Scene::Start()
	{
		m_InitCallbacks.Sort([](const auto& a, const auto& b)->bool
			{
				return a.Key() < b.Key();
			});

		for (const auto& [prio, callback] : m_InitCallbacks)
			callback(*this);

		//for (GameObject* pG : m_GameObjects)
		//	pG->Awake();

		//for (GameObject* pG : m_GameObjects)
		//{
		//	pG->Start();

		//	if (pG->HasTag("MainCamera"))
		//		if (CameraComponent* pC{ pG->GetComponentByType<CameraComponent>() }; pC != nullptr)
		//			m_pActiveCamera = pC;
		//}

		//if (!m_pActiveCamera)
		//{
		//	GameObject* pCamera{ Instantiate("MainCamera", this) };
		//	m_pActiveCamera = pCamera->AddComponent(new CameraComponent
		//		{
		//			pCamera,
		//			0.1f,
		//			100.f,
		//			Math::ToRadians(45.f),
		//			static_cast<float>(Core::GetInstance().GetWindowWidth()) / Core::GetInstance().GetWindowHeight()
		//		});

		//	pCamera->pTransform->Translate(Math::Vec3D{ 0.f, 0.f, -10.f }, true);

		//	pCamera->Start();

		//	AddGameObject(pCamera);
		//}
	}

	void Scene::Update()
	{
		//for (GameObject* pG : m_GameObjects)
		//	pG->FixedUpdate();

		//for (GameObject* pG : m_GameObjects)
		//	pG->Update();

		//for (GameObject* pG : m_GameObjects)
		//	pG->LateUpdate();

		//m_GameObjects.EraseAll([](const GameObject* pG)->bool
		//	{
		//		if (pG->IsMarkedForDestruction())
		//		{
		//			__DELETE(pG);
		//			return true;
		//		}

		//		return false;
		//	});
	}

	void Scene::Render() const
	{
		// for (const GameObject* pG : m_GameObjects)
		// 	pG->Render();
	}
}