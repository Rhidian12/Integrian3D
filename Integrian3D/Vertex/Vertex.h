#pragma once

#include "EngineConstants.h"
#include "Math/Math.h"

namespace Integrian3D
{
	struct Vertex final
	{
		Vertex() = default;
		explicit Vertex(const Math::Vec3D& position)
			: Position{ position }
			, UV{}
		{}
		Vertex(const Math::Vec3D& position, const Math::Vec2D& uv)
			: Position{ position }
			, UV{ uv }
		{}

		Vertex(const Vertex& Other) noexcept
			: Position{ Other.Position}
			, UV{Other.UV}
		{}
		Vertex(Vertex&& Other) noexcept
			: Position{ __MOVE(Other.Position) }
			, UV{ __MOVE(Other.UV) }
		{}
		Vertex& operator=(const Vertex& Other) noexcept
		{
			Position = Other.Position;
			UV = Other.UV;

			return *this;
		}
		Vertex& operator=(Vertex&& Other) noexcept
		{
			Position = __MOVE(Other.Position);
			UV = __MOVE(Other.UV);

			return *this;
		}

		Math::Vec3D Position;
		Math::Vec2D UV;
	};
}