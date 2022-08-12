#pragma once

#include "Math/RGBColour.h"

#include <cstdint> /* uint64_t */
#include <limits> /* std::numeric_limits */
#include <bitset> /* std::bitset */

#undef max

namespace Integrian3D
{
	using Entity = uint32_t; /* This supports ~4 billion possible entities, seems like enough */
	using ComponentType = uint16_t; /* This supports ~56k possible component types, seems like enough */

	constexpr Entity MaxEntities{ std::numeric_limits<Entity>::max() - 1 };
	constexpr ComponentType MaxComponentTypes{ std::numeric_limits<ComponentType>::max() - 1 };

	using EntitySignature = std::bitset<MaxComponentTypes>;

	constexpr Entity InvalidEntityID{ std::numeric_limits<Entity>::max() };
	constexpr ComponentType InvalidComponentID{ std::numeric_limits<ComponentType>::max() };
}