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

		/* Does not follow naming conventions, only an exception for usability sake */
		class TransformComponent* pTransform;

		/* [TODO]: Add Clone() function */

#pragma region Component_Functionality

		template<typename T>
		T* const AddComponent(T* const pComponent);

		template<typename T>
		__NODISCARD T* const GetComponentByType() const;

#pragma endregion

#pragma region Tag_Functionality

		const std::string& AddTag(const std::string& tag);

		const std::string& SetTag(const std::string& tag, const uint64_t index);

		void RemoveTag(const std::string& tag);

		void RemoveTag(const uint64_t index);

		__NODISCARD const std::string& GetTag(const uint64_t index) const;

		__NODISCARD const TArray<std::string>& GetTags() const;

		__NODISCARD bool HasTag(const std::string& tag) const;

#pragma endregion

#pragma region Internal_Functionality

		void Start();
	
		void Update();
		
		void Render() const;

#pragma endregion

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
		TArray<std::string> m_Tags;
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