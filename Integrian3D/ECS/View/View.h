#pragma once

#include "../../EngineConstants.h"

#include "../ComponentArray/ComponentArray.h"
#include "../../SparseSet/SparseSet.h"

#include <functional>
#include <utility>
#include <tuple>

namespace Integrian3D
{
	template<typename ... Ts>
	class View final
	{
		using ViewContainerType = std::tuple<ComponentArray<Ts>&...>;

	public:
		View() = default;
		View(ViewContainerType&& components, const SparseSet<Entity>& entities)
			: m_Components{ std::move(components) }
			, Entities{ entities }
		{}

		View(const View&) noexcept = delete;
		View& operator=(const View&) noexcept = delete;

		void ForEach(const std::function<void(Ts&...)>& function) const
		{
			auto indexSequence{ std::make_index_sequence<sizeof ... (Ts)>{} };

			for (const Entity entity : Entities)
			{
				ForEachImpl(function, entity, indexSequence);
			}
		}

		// Warning: This is an EXPENSIVE function, only use when absolutely necessary!
		int32 GetNrOfEntities() const
		{
			auto indexSequence{ std::make_index_sequence<sizeof ... (Ts)>{} };
			int32 Count{};

			for (const Entity Ent : Entities)
			{
				GetNrOfEntitiesImpl(Ent, Count, indexSequence);
			}

			return Count;
		}

	private:
		template<size_t ... Is>
		void ForEachImpl(const std::function<void(Ts&...)>& function, const Entity ent, const std::index_sequence<Is...>&) const
		{
			if ((ent != InvalidEntityID) && (std::get<Is>(m_Components).HasEntity(ent) && ...))
			{
				std::apply(function, std::tuple<Ts&...>(std::get<Is>(m_Components).GetComponent(ent)...));
			}
		}

		template<size_t ... Is>
		void GetNrOfEntitiesImpl(const Entity Ent, int32& Count, const std::index_sequence<Is...>&) const
		{
			if ((Ent != InvalidEntityID) && (std::get<Is>(m_Components).HasEntity(Ent) && ...))
			{
				++Count;
			}
		}

		ViewContainerType m_Components;
		const SparseSet<Entity>& Entities;
	};
}