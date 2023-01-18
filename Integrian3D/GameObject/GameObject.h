#pragma once

#include "../EngineConstants.h"
#include "../Object/Object.h"
#include "../Array/Array.h"
#include "GameObjectUtils.h"

#include <string> /* std::string */

namespace Integrian3D
{
	class GameObject : public Object
	{
	public:
		virtual ~GameObject();

		template<typename T>
		T* const AddComponent(T* const pComponent);

		template<typename T>
		__NODISCARD T* const GetComponentByType() const;

		void Start();
		void Update();
		void Render() const;

		/* Does not follow naming conventions, only an exception for usability sake */
		class TransformComponent* pTransform;

	protected:
		GameObject();
		explicit GameObject(const std::string& name);

		friend GameObject* Instantiate(const std::string&);
		friend GameObject* Instantiate(const std::string&, class Scene*);

		struct ComponentInfo final
		{
			uint32_t ID;
			class Component* pComponent;
		};

		TArray<ComponentInfo> m_Components;
	};

#pragma region Templated_Functions

	template<typename T>
	T* const GameObject::AddComponent(T* const pComponent)
	{
		constexpr uint32_t id{ GOUtils::GenerateComponentID<T>() };

		if (m_Components.Find([id](const ComponentInfo& info)->bool
			{
				return info.ID == id;
			}) == m_Components.cend())
		{
			m_Components.Add(ComponentInfo{ id, pComponent });

			return pComponent;
		}

			return nullptr;
	}

	template<typename T>
	T* const GameObject::GetComponentByType() const
	{
		constexpr uint32_t id{ GOUtils::GenerateComponentID<T>() };
		const auto cIt{ m_Components.Find([id](const ComponentInfo& info)->bool
			{
				return info.ID == id;
			}) };

		if (cIt != m_Components.cend())
			return static_cast<T*>(cIt->pComponent);

		return nullptr;
	}

#pragma endregion

#pragma region Helper_Functions

	__NODISCARD GameObject* Instantiate(const std::string& name = "");
	__NODISCARD GameObject* Instantiate(const std::string& name, class Scene* pScene);

#pragma endregion
}