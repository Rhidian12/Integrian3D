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
		T& AddComponent()
		{
			return Components.emplace_back(T{});
		}
		template<typename ... Ts>
		T& AddComponent(Ts&& ... args)
		{
			return Components.emplace_back(T{ std::forward<Ts>(args)... });
		}

		virtual void Remove(const Entity entity) override
		{
			Components.erase(Components.begin() + entity);
		}

		__NODISCARD T& GetComponent(const Entity entity) { return Components[entity]; }
		__NODISCARD const T& GetComponent(const Entity entity) const { return Components[entity]; }

		__NODISCARD std::vector<T>& GetComponents() { return Components; }
		__NODISCARD const std::vector<T>& GetComponents() const { return Components; }

	private:
		std::vector<T> Components;
	};
}