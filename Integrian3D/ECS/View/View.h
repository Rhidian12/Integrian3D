#pragma once

#include "../../EngineConstants.h"
#include "../DoubleStorage/DoubleStorage.h"

#include <vector> /* std::vector */
#include <functional> /* std::function, std::reference_wrapper */

namespace Integrian3D
{
	template<typename ... Ts>
	class View final
	{
		using ViewContainerType = std::tuple<DoubleStorage<Entity, Ts>&...>;

	public:
		explicit View(ViewContainerType&& components, std::unordered_map<Entity, EntitySignature>& sigs)
			: Components{ __MOVE(ViewContainerType, components) }
			, EntitySignatures{ sigs }
			, Entities{ std::get<0>(Components).GetKeys() }
		{
			SetEntities(std::make_index_sequence<sizeof ... (Ts)>{});
		}

		void ForEach(const std::function<void(Ts&...)>& function)
		{
			auto indexSequence{ std::make_index_sequence<sizeof ... (Ts)>{} };

			for (Entity entity : Entities)
			{
				ForEach(function, __MOVE(Entity, entity), indexSequence);
			}
		}

	private:
		template<size_t ... Indices>
		void ForEach(const std::function<void(Ts&...)>& function, Entity&& ent, std::index_sequence<Indices...>)
		{
			if ((EntitySignatures.at(ent).test(GenerateComponentID<Ts>()) && ...))
			{
				auto tuple{ std::tuple<Ts&...>(std::get<Indices>(Components).GetValue(ent)...) };
				std::apply(function, tuple);
			}
		}

		template<size_t ... Is>
		void SetEntities(std::index_sequence<Is...>)
		{
			((Entities = std::get<Is>(Components).GetKeys().size() < Entities.size() ? std::get<Is>(Components).GetKeys() : Entities), ...);
		}

		ViewContainerType Components;
		std::unordered_map<Entity, EntitySignature>& EntitySignatures;
		std::vector<Entity> Entities;
	};
}