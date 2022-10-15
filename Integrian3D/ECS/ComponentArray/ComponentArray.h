#pragma once

#include "../../EngineConstants.h"

#include <vector> /* std::vector */

namespace Integrian3D
{
	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;

		virtual void Remove(const Entity entity) = 0;
	};

	template<typename T>
	class ComponentArray final : public IComponentArray
	{
	public:
		ComponentArray(const size_t nrOfComponentsPerList)
			: Entities{}
			, Components{}
			, NrOfComponentsPerList{ nrOfComponentsPerList }
			, NrOfEntities{}
		{}

		ComponentArray(const ComponentArray&) noexcept = delete;
		ComponentArray(ComponentArray&& other) noexcept
			: Entities{ __MOVE(std::vector<std::vector<Entity>>, other.Entities) }
			, Components{ __MOVE(std::vector<std::vector<T>>, other.Components) }
			, NrOfComponentsPerList{ __MOVE(size_t, other.NrOfComponentsPerList) }
			, NrOfEntities{ __MOVE(size_t, other.NrOfEntities) }
		{
			other.Components.clear();
			other.Entities.clear();
		}

		ComponentArray& operator=(const ComponentArray&) noexcept = delete;
		ComponentArray& operator=(ComponentArray&& other) noexcept
		{
			Entities = __MOVE(std::vector<std::vector<Entity>>, other.Entities);
			Components = __MOVE(std::vector<std::vector<T>>, other.Components);
			NrOfComponentsPerList = __MOVE(size_t, other.NrOfComponentsPerList);
			NrOfEntities = __MOVE(size_t, other.NrOfEntities);

			other.Components.clear();
			other.Entities.clear();

			return *this;
		};

		T& AddComponent(const Entity entity)
		{
			while (entity / NrOfComponentsPerList >= Components.size())
			{
				Entities.push_back(std::vector<Entity>{});
				Components.push_back(std::vector<T>{});
			}

			++NrOfEntities;

			Entities[entity / NrOfComponentsPerList].emplace_back(entity);
			return Components[entity / NrOfComponentsPerList].emplace_back(T{});
		}
		template<typename ... Ts>
		T& AddComponent(const Entity entity, Ts&& ... args)
		{
			while (entity / NrOfComponentsPerList >= Components.size())
			{
				Entities.push_back(std::vector<Entity>{});
				Components.push_back(std::vector<T>{});
			}

			++NrOfEntities;

			Entities[entity / NrOfComponentsPerList].emplace_back(entity);
			return Components[entity / NrOfComponentsPerList].emplace_back(T{ std::forward<Ts>(args)... });
		}

		virtual void Remove(const Entity entity) override
		{
			--NrOfEntities;

			Components[entity / NrOfComponentsPerList].erase(
				Components[entity / NrOfComponentsPerList].begin() +
				(std::find(
					Entities[entity / NrOfComponentsPerList].cbegin(),
					Entities[entity / NrOfComponentsPerList].cend(),
					entity) -
					Entities[entity / NrOfComponentsPerList].cbegin()));
		}

		__NODISCARD T& GetComponent(const Entity entity)
		{
			return Components[entity / NrOfComponentsPerList][
				std::find(
					Entities[entity / NrOfComponentsPerList].cbegin(),
					Entities[entity / NrOfComponentsPerList].cend(),
					entity) -
					Entities[entity / NrOfComponentsPerList].cbegin()];
		}
		__NODISCARD const T& GetComponent(const Entity entity) const
		{
			return Components[entity / NrOfComponentsPerList][
				std::find(
					Entities[entity / NrOfComponentsPerList].cbegin(),
					Entities[entity / NrOfComponentsPerList].cend(),
					entity) -
					Entities[entity / NrOfComponentsPerList].cbegin()];
		}

		__NODISCARD std::vector<std::vector<T>>& GetComponents() { return Components; }
		__NODISCARD const std::vector<std::vector<T>>& GetComponents() const { return Components; }

		__NODISCARD std::vector<std::vector<Entity>>& GetEntities() { return Entities; }
		__NODISCARD const std::vector<std::vector<Entity>>& GetEntities() const { return Entities; }

		__NODISCARD size_t GetNrOfEntities() const { return NrOfEntities; }

	private:
		std::vector<std::vector<Entity>> Entities;
		std::vector<std::vector<T>> Components;
		size_t NrOfComponentsPerList;
		size_t NrOfEntities;
	};
}