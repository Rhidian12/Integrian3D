#pragma once

#include "Math/Point2.h"
#include "Math/Point3.h"
#include "Math/Point4.h"
#include "Math/Vector3.h"
#include "Math/RGBColour.h"
#include "Math/Matrix.h"

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

	using Point2f = Point2<float>;
	using Point2d = Point2<double>;

	using Point3f = Point3<float>;
	using Point3d = Point3<double>;

	using Point4f = Point4<float>;
	using Point4d = Point4<double>;

	using Vector3f = Vector3<float>;
	using Vector3d = Vector3<double>;

	using Matrix3f = Matrix<3, 3, float>;
	using Matrix3d = Matrix<3, 3, double>;

	using Matrix4f = Matrix<4, 4, float>;
	using Matrix4d = Matrix<4, 4, double>;
}