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
		, m_IsMarkedForDestruction{}
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

	void GameObject::Destroy()
	{
		m_IsMarkedForDestruction = true;
		m_IsActive = false;
	}

	bool GameObject::IsMarkedForDestruction() const
	{
		return m_IsMarkedForDestruction;
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
			static std::string errorString{ "__INVALID__" };
			return errorString;
		}

		return m_Tags[index];
	}

	const TArray<std::string>& GameObject::GetTags() const
	{
		return m_Tags;
	}

	bool GameObject::HasTag(const std::string& tag) const
	{
		return m_Tags.Find(tag) != m_Tags.cend();
	}

	void GameObject::Awake()
	{
		for (const ComponentInfo& info : m_Components)
			info.pComponent->Awake();
	}

	void GameObject::Start()
	{
		for (const ComponentInfo& info : m_Components)
			if (info.pComponent->IsActive())
				info.pComponent->Start();
	}

	void GameObject::FixedUpdate()
	{
		for (const ComponentInfo& info : m_Components)
			if (info.pComponent->IsActive())
				info.pComponent->FixedUpdate();
	}

	void GameObject::Update()
	{
		if (!m_IsActive)
			return;

		for (const ComponentInfo& info : m_Components)
			if (info.pComponent->IsActive())
				info.pComponent->Update();
	}

	void GameObject::LateUpdate()
	{
		for (const ComponentInfo& info : m_Components)
			if (info.pComponent->IsActive())
				info.pComponent->Update();
	}

	void GameObject::Render() const
	{
		if (!m_IsActive)
			return;

		for (const ComponentInfo& info : m_Components)
			if (info.pComponent->IsActive())
				info.pComponent->Render();
	}
}