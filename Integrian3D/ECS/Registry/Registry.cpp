#include "Registry.h"

namespace Integrian3D
{
	using EMap = std::unordered_map<Entity, EntitySignature>;
	using CPools = std::unordered_map<ComponentType, std::unique_ptr<IComponentArray>>;

	Registry::Registry()
		: EntitySignatures{}
		, Entities{}
		, CurrentEntityCounter{}
	{}

	Registry::~Registry()
	{
		for (int i{ static_cast<int>(Entities.Size()) - 1 }; i >= 0; --i)
		{
			ReleaseEntity(Entities[i]);
		}
	}

	Registry::Registry(Registry&& other) noexcept
		: EntitySignatures{ __MOVE(EMap, other.EntitySignatures) }
		, Entities{ __MOVE(SparseSet<Entity>, other.Entities) }
		, CurrentEntityCounter{ __MOVE(Entity, other.CurrentEntityCounter) }
		, ComponentPools{ __MOVE(CPools, other.ComponentPools) }
	{
		other.EntitySignatures.clear();
		other.Entities.Clear();
		other.CurrentEntityCounter = 0;
		other.ComponentPools.clear();
	}

	Registry& Registry::operator=(Registry&& other) noexcept
	{
		EntitySignatures = __MOVE(EMap, other.EntitySignatures);
		Entities = __MOVE(SparseSet<Entity>, other.Entities);
		CurrentEntityCounter = __MOVE(Entity, other.CurrentEntityCounter);
		ComponentPools = __MOVE(CPools, other.ComponentPools);

		other.EntitySignatures.clear();
		other.Entities.Clear();
		other.CurrentEntityCounter = 0;
		other.ComponentPools.clear();

		return *this;
	}

	Entity Registry::CreateEntity()
	{
		assert(CurrentEntityCounter <= MaxEntities);

		const Entity entity(CurrentEntityCounter++);
		Entities.Add(entity);

		EntitySignatures.insert(std::make_pair(entity, EntitySignature{}));

		return entity;
	}

	bool Registry::ReleaseEntity(Entity entity)
	{
		if (Entities.Contains(entity))
		{
			RemoveAllComponents(entity, GetEntitySignature(entity));

			EntitySignatures.erase(entity);

			Entities.Remove(entity);

			--CurrentEntityCounter;

			return true;
		}

		return false;
	}

	void Registry::RemoveAllComponents(const Entity entity, const EntitySignature& sig)
	{
		for (ComponentType i{}; i < MaxComponentTypes; ++i)
		{
			if (sig.test(i))
			{
				assert(ComponentPools[i]);

				ComponentPools[i]->Remove(Entities.GetIndex(entity));
			}
		}
	}
}