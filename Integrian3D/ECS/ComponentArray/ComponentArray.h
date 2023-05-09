#pragma once

#include "../../EngineConstants.h"

#include "../DenseSet/DenseSet.h"

#include <vector>

namespace Integrian3D
{
	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;

		virtual void Remove(const Entity entity) = 0;
		virtual void RemoveAll() = 0;
	};

	template<typename T>
	class ComponentArray final : public IComponentArray
	{
	public:
		ComponentArray() = default;

		ComponentArray(const ComponentArray&) noexcept = delete;
		ComponentArray(ComponentArray&&) noexcept = default;
		ComponentArray& operator=(const ComponentArray&) noexcept = delete;
		ComponentArray& operator=(ComponentArray&&) noexcept = default;

		T& AddComponent(const Entity entity)
		{
			m_Entities.Add(entity, static_cast<Entity>(m_Components.size()));
			return m_Components.emplace_back(T{});
		}
		template<typename ... Ts>
		T& AddComponent(const Entity entity, Ts&& ... args)
		{
			m_Entities.Add(entity, static_cast<Entity>(m_Components.size()));
			return m_Components.emplace_back(T{ std::forward<Ts>(args)... });
		}

		virtual void Remove(const Entity entity) override
		{
			m_Entities.Remove(entity);
		}

		virtual void RemoveAll() override
		{
			m_Entities.Clear();
			m_Components.clear();
		}

		__NODISCARD Entity FindEntity(const T& comp) const
		{
			__ASSERT(std::find(m_Components.cbegin(), m_Components.cend(), comp) != m_Components.cend());

			return m_Entities.GetFirst(std::distance(m_Components.cbegin(), std::find(m_Components.cbegin(), m_Components.cend(), comp)));
		}

		__NODISCARD bool HasEntity(const Entity entity) const
		{
			return m_Entities.Contains(entity);
		}

		__NODISCARD T& GetComponent(const Entity entity)
		{
			return m_Components[m_Entities.GetSecond(entity)];
		}
		__NODISCARD const T& GetComponent(const Entity entity) const
		{
			return m_Components[m_Entities.GetSecond(entity)];
		}

	private:
		DenseSet<Entity> m_Entities;
		std::vector<T> m_Components;
	};
}