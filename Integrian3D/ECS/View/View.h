#pragma once

#include "../../EngineConstants.h"
#include "../Memory/Memory.h"

#include <vector> /* std::vector */
#include <assert.h> /* assert() */
#include <utility> /* std::move(), ... */
#include <functional> /* std::function, std::reference_wrapper */
#include <array> /* std::array */

namespace Integrian3D
{
	template<typename ... TComponents>
	class View final
	{
		using ViewContainerType = std::tuple<std::vector<std::reference_wrapper<TComponents>, STLAllocator<std::reference_wrapper<TComponents>, StackAllocator>>...>;
		using EntitiesContainer = std::array<std::vector<Entity>*, sizeof ... (TComponents)>;

	public:
		explicit View(ViewContainerType&& components, EntitiesContainer&& entities, std::unordered_map<Entity, EntitySignature>& sigs)
			: Components{ std::move(components) }
			, Entities{ *entities[0] }
			, EntitySignatures{ sigs }
		{
			size_t nrOfEntities{ Entities.size() };
			for (size_t i{ 1 }; i < sizeof ... (TComponents); ++i)
			{
				if (entities[i]->size() < nrOfEntities)
				{
					nrOfEntities = entities[i]->size();
					Entities = *(entities[i]);
				}
			}
		}

		void ForEach(const std::function<void(TComponents&...)>& function)
		{
			auto indexSequence{ std::make_index_sequence<sizeof ... (TComponents)>{} };

			size_t index{};
			for (Entity entity : Entities)
			{
				ForEach(function, std::move(entity), index++, indexSequence);
			}
		}

	private:
		template<size_t ... Indices>
		void ForEach(const std::function<void(TComponents&...)>& function, Entity&& ent, size_t index, std::index_sequence<Indices...>)
		{
			if ((EntitySignatures.at(ent).test(GenerateComponentID<TComponents>()) && ...))
			{
				auto tuple{ std::tuple<TComponents&...>(std::get<Indices>(Components)[index].get()...) };
				std::apply(function, tuple);
			}
		}

		ViewContainerType Components;
		std::vector<Entity> Entities;
		std::unordered_map<Entity, EntitySignature>& EntitySignatures;
	};
}