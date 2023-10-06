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
			, Normal{}
			, UV{}
		{}
		Vertex(const Math::Vec3D& position, const Math::Vec3D& Normal, const Math::Vec2D& uv)
			: Position{ position }
			, Normal{ Normal }
			, UV{ uv }
		{}
		Vertex(const Math::Vec3D& position, const Math::Vec2D& uv)
			: Position{ position }
			, Normal{}
			, UV{ uv }
		{}

		Vertex(const Vertex& Other) noexcept
			: Position{ Other.Position }
			, Normal{ Other.Normal }
			, UV{ Other.UV }
		{}
		Vertex(Vertex&& Other) noexcept
			: Position{ __MOVE(Other.Position) }
			, Normal{ __MOVE(Other.Normal) }
			, UV{ __MOVE(Other.UV) }
		{}
		Vertex& operator=(const Vertex& Other) noexcept
		{
			Position = Other.Position;
			Normal = Other.Normal;
			UV = Other.UV;

			return *this;
		}
		Vertex& operator=(Vertex&& Other) noexcept
		{
			Position = __MOVE(Other.Position);
			Normal = __MOVE(Other.Normal);
			UV = __MOVE(Other.UV);

			return *this;
		}

		Math::Vec3D Position;
		Math::Vec3D Normal;
		Math::Vec2D UV;
	};
}