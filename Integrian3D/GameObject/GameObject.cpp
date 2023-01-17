#include "GameObject.h"
#include "../SceneManager/SceneManager.h"
#include "../DebugUtility/DebugUtility.h"
#include "../Scene/Scene.h"

namespace Integrian3D
{
	GameObject::GameObject()
		: Object{}
	{}

	GameObject::GameObject(const std::string& name)
		: Object{ name }
	{}

	GameObject::~GameObject()
	{
		for (Component* pC : m_Components)
		{
			__DELETE(pC);
		}

		m_Components.Clear();
	}

	void GameObject::AddComponent(Component* const pComp)
	{
		if (m_Components.Find(pComp) == m_Components.cend())
			m_Components.Add(pComp);
	}

	void GameObject::Update()
	{
		for (Component* pC : m_Components)
			pC->Update();
	}

	void GameObject::Render() const
	{
		for (Component* pC : m_Components)
			pC->Render();
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