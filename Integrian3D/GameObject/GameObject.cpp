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
		, m_Tags{}
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

	const std::string& GameObject::AddTag(const std::string& tag)
	{
		m_Tags.Add(tag);

		return tag;
	}

	const std::string& GameObject::SetTag(const std::string& tag, const uint64_t index)
	{
		if (index < m_Tags.Size())
			m_Tags[index] = tag;
		else
			Debug::LogWarning("GameObject::SetTag() > Index is out of bounds", false);

		return tag;
	}

	void GameObject::RemoveTag(const std::string& tag)
	{
		m_Tags.Erase(tag);
	}

	void GameObject::RemoveTag(const uint64_t index)
	{
		if (index >= m_Tags.Size())
		{
			Debug::LogWarning("GameObject::RemoveTag() > Index is out of bounds", false);
			return;
		}

		m_Tags.EraseByIndex(index);
	}

	const std::string& GameObject::GetTag(const uint64_t index) const
	{
		if (index >= m_Tags.Size())
		{
			Debug::LogWarning("GameObject::GetTag() > Index is out of bounds", false);
			return;
		}

		return m_Tags[index];
	}

	const TArray<std::string>& GameObject::GetTags() const
	{
		return m_Tags;
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