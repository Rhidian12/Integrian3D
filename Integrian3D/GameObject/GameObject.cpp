#include "GameObject.h"
#include "../SceneManager/SceneManager.h"
#include "../DebugUtility/DebugUtility.h"
#include "../Scene/Scene.h"
#include "../Component/Component.h"
#include "../Components/TransformComponent/TransformComponent.h"

namespace Integrian3D
{
	GameObject::GameObject()
		: GameObject{ "" }
	{}

	GameObject::GameObject(const std::string& name)
		: Object{ name }
		, pTransform{}
		, m_Components{}
	{
		pTransform = AddComponent(new TransformComponent{ this });
	}

	GameObject::~GameObject()
	{
		for (ComponentInfo& info : m_Components)
		{
			info.ID = 0;
			__DELETE(info.pComponent);
		}

		m_Components.Clear();
	}

	void GameObject::Start()
	{
		for (const ComponentInfo& info : m_Components)
			info.pComponent->Start();
	}

	void GameObject::Update()
	{
		for (const ComponentInfo& info : m_Components)
			info.pComponent->Update();
	}

	void GameObject::Render() const
	{
		for (const ComponentInfo& info : m_Components)
			info.pComponent->Render();
	}

#pragma region Helper_Functions

	GameObject* Instantiate(const std::string& name)
	{
		Scene* const pActiveScene{ SceneManager::GetInstance().GetActiveScene() };

		if (!pActiveScene)
		{
			Debug::LogError("Instantiate(std::string) > No scene has been added, add a scene first", false);
			return nullptr;
		}

		GameObject* pG{ new GameObject{ name } };

		pActiveScene->AddGameObject(pG);

		return pG;
	}

	GameObject* Instantiate(const std::string& name, Scene* pScene)
	{
		if (!pScene)
		{
			Debug::LogError("Instantiate(std::string, Scene*) > Given scene is a nullptr", false);
			return nullptr;
		}

		GameObject* pG{ new GameObject{ name } };

		pScene->AddGameObject(pG);

		return pG;
	}

#pragma endregion
}