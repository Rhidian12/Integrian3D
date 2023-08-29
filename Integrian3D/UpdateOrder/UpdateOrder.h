#pragma once
#include <cstdint>

namespace Integrian3D
{
	enum class UpdateOrder : uint32_t
	{
		PrePhysics = 1 << 4,
		Physics = 1 << 5,
		PostPhysics = 1 << 6,

		PreUpdate = 1 << 7,
		Update = 1 << 8,
		PostUpdate = 1 << 9,
	};
}