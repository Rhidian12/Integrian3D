#pragma once

#include "../../EngineConstants.h"

#include "../ComponentArray/ComponentArray.h"
#include "../ComponentIDGenerator.h"
#include "../View/View.h"
#include "../../SparseSet/SparseSet.h"

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
		__NODISCARD View<Ts...> CreateView()
		{
			/* Get all components asked for by the user */
			std::tuple<ComponentArray<Ts>&...> comps
			{
				(*static_cast<ComponentArray<Ts>*>(GetComponentArray(ECS::GenerateComponentID<Ts>()).get()))...
			};

			return View<Ts...>{ std::move(comps), EntitySignatures };
		}

		template<typename T>
		T& AddComponent(const Entity entity)
		{
			SetEntitySignature(entity, ECS::GenerateComponentID<T>());

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
			SetEntitySignature(entity, ECS::GenerateComponentID<T>());

			std::unique_ptr<IComponentArray>& pool{ GetComponentArray(GenerateComponentID<T>()) };

			if (!pool)
			{
				pool.reset(new ComponentArray<T>{});
			}

			return static_cast<ComponentArray<T>*>(pool.get())->AddComponent<Ts...>(entity, std::forward<Ts>(args)...);
		}

		template<typename T>
		void RemoveComponent(const Entity entity)
		{
			__ASSERT(HasEntity(entity));

			GetComponentArray(ECS::GenerateComponentID<T>())->Remove(entity);
			SetEntitySignature(entity, ECS::GenerateComponentID<T>(), false);
		}

		template<typename T>
		__NODISCARD bool HasComponent(const Entity entity) const 
		{
			return GetEntitySignature(entity).test(ECS::GenerateComponentID<T>());
		}

		template<typename T>
		__NODISCARD T& GetComponent(const Entity entity)
		{
			__ASSERT(GetComponentArray(ECS::GenerateComponentID<T>()));
			return static_cast<ComponentArray<T>*>(GetComponentArray(ECS::GenerateComponentID<T>()).get())->GetComponent(entity);
		}
		template<typename T>
		__NODISCARD const T& GetComponent(const Entity entity) const
		{
			__ASSERT(GetComponentArray(ECS::GenerateComponentID<T>()));
			return static_cast<ComponentArray<T>*>(GetComponentArray(ECS::GenerateComponentID<T>()).get())->GetComponent(entity);
		}

		__NODISCARD Entity CreateEntity();
		__NODISCARD size_t GetAmountOfEntities() const { return Entities.Size(); }
		__NODISCARD bool HasEntity(const Entity entity) const;
		bool ReleaseEntity(const Entity entity);

		void Clear();

		__NODISCARD const EntitySignature& GetEntitySignature(const Entity entity) const;

	private:
		void SetEntitySignature(const Entity entity, const size_t id, const bool val = true);

		void RemoveAllComponents(const Entity entity, const EntitySignature& sig);
		__NODISCARD std::unique_ptr<IComponentArray>& GetComponentArray(const size_t cType);

		// Entities
		std::vector<std::pair<Entity, EntitySignature>> EntitySignatures; // [TODO]: Make a map that uses arrays 
		SparseSet<Entity> Entities;
		std::vector<Entity> RecycledEntities;
		Entity CurrentEntityCounter;

		// Components
		std::vector<std::pair<size_t, std::unique_ptr<IComponentArray>>> ComponentPools; // [TODO]: Make a map that uses arrays 
	};
}