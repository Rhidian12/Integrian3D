#pragma once

#include "../../EngineConstants.h"
#include "../DoubleStorage/DoubleStorage.h"

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
		T& AddComponent(const Entity entity)
		{
			return Components.Add(entity, T{});
		}
		template<typename ... Ts>
		T& AddComponent(const Entity entity, Ts&& ... args)
		{
			return Components.Add(entity, T{ std::forward<Ts>(args)... });
		}

		virtual void Remove(const Entity entity) override
		{
			Components.Remove(entity);
		}

		__NODISCARD T& GetComponent(const Entity entity) { return Components.GetValue(entity); }
		__NODISCARD const T& GetComponent(const Entity entity) const { return Components.GetValue(entity); }

		__NODISCARD std::vector<T>& GetComponents() { return Components.GetValues(); }
		__NODISCARD const std::vector<T>& GetComponents() const { return Components.GetValues(); }

	private:
		DoubleStorage<Entity, T> Components;
	};
}