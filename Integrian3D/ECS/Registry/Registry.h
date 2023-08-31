#pragma once

#include "../../EngineConstants.h"

#include "../ComponentIDGenerator.h"
#include "../View/View.h"
#include "../../SparseSet/SparseSet.h"

#include <vector>

namespace Integrian3D
{
	class Registry final
	{
	public:
		Registry();
		~Registry();

		Registry(const Registry&) noexcept = delete;
		Registry(Registry&& other) noexcept;
		Registry& operator=(const Registry&) noexcept = delete;
		Registry& operator=(Registry&& other) noexcept;

		template<typename ... Ts>
		__NODISCARD View<Ts...> CreateView() const
		{
			/* Get all components asked for by the user */
			std::tuple<ComponentArray<Ts>&...> comps
			{
				(*static_cast<ComponentArray<Ts>*>(GetComponentArray(ECS::GenerateComponentID<Ts>()).get()))...
			};

			return View<Ts...>(std::move(comps), Entities);
		}

		template<typename T>
		T& AddComponent(const Entity entity)
		{
			std::unique_ptr<IComponentArray>& pool{ GetComponentArray(ECS::GenerateComponentID<T>())};

			if (!pool)
			{
				pool.reset(new ComponentArray<T>{});
			}

			return static_cast<ComponentArray<T>*>(pool.get())->AddComponent(entity);
		}
		template<typename T, typename ... Ts>
		T& AddComponent(const Entity entity, Ts&& ... args)
		{
			std::unique_ptr<IComponentArray>& pool{ GetComponentArray(ECS::GenerateComponentID<T>()) };

			if (!pool)
			{
				pool.reset(new ComponentArray<T>{});
			}

			return static_cast<ComponentArray<T>*>(pool.get())->AddComponent<Ts...>(entity, std::forward<Ts>(args)...);
		}

		template<typename T>
		void RemoveComponent(const Entity entity)
		{
			__ASSERT(HasEntity(entity), "Registry::RemoveComponent > Entity %i is not present in registry", entity);

			GetComponentArray(ECS::GenerateComponentID<T>())->Remove(entity);
		}

		template<typename T>
		__NODISCARD bool HasComponent(const Entity entity) const 
		{
			return static_cast<ComponentArray<T>*>(GetComponentArray(ECS::GenerateComponentID<T>()).get())->HasEntity(entity);
		}

		template<typename T>
		__NODISCARD T& GetComponent(const Entity entity)
		{
			__ASSERT(HasEntity(entity), "Registry::GetComponent > Entity %i is not present in registry", entity);
			__CHECK(GetComponentArray(ECS::GenerateComponentID<T>()));

			return static_cast<ComponentArray<T>*>(GetComponentArray(ECS::GenerateComponentID<T>()).get())->GetComponent(entity);
		}
		template<typename T>
		__NODISCARD const T& GetComponent(const Entity entity) const
		{
			__ASSERT(HasEntity(entity), "Registry::GetComponent > Entity %i is not present in registry", entity);
			__CHECK(GetComponentArray(ECS::GenerateComponentID<T>()));

			return static_cast<ComponentArray<T>*>(GetComponentArray(ECS::GenerateComponentID<T>()).get())->GetComponent(entity);
		}

		template<typename T>
		__NODISCARD Entity FindEntity(const T& comp)
		{
			__CHECK(GetComponentArray(ECS::GenerateComponentID<T>()));
			return static_cast<ComponentArray<T>*>(GetComponentArray(ECS::GenerateComponentID<T>()).get())->FindEntity(comp);
		}

		__NODISCARD Entity CreateEntity();
		__NODISCARD size_t GetAmountOfEntities() const { return Entities.Size(); }
		__NODISCARD bool HasEntity(const Entity entity) const;
		bool ReleaseEntity(const Entity entity);

		template<typename ... Ts>
		__NODISCARD bool CanViewBeCreated() const
		{
			return ((std::find_if(ComponentPools.cbegin(), ComponentPools.cend(), [](const auto& cPool)->bool
					{
						return ECS::GenerateComponentID<Ts>() == cPool.first;
					}) != ComponentPools.cend()) && ...);
		}

		void Clear();

	private:
		void RemoveAllComponents(const Entity entity);
		__NODISCARD std::unique_ptr<IComponentArray>& GetComponentArray(const size_t cType);
		__NODISCARD const std::unique_ptr<IComponentArray>& GetComponentArray(const size_t cType) const;

		// Entities
		SparseSet<Entity> Entities;
		std::vector<Entity> RecycledEntities;
		Entity CurrentEntityCounter;

		// Components
		std::vector<std::pair<size_t, std::unique_ptr<IComponentArray>>> ComponentPools; // [TODO]: Make a map that uses arrays 
	};
}