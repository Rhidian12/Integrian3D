#pragma once

#include "Math/Math.h"
#include <raylib.h>

__NODISCARD Vector2 ToRay2(const Integrian3D::Math::Vec2F& Vector);
__NODISCARD Vector2 ToRay2(const Integrian3D::Math::Vec2D& Vector);

__NODISCARD Vector3 ToRay3(const Integrian3D::Math::Vec3F& Vector);
__NODISCARD Vector3 ToRay3(const Integrian3D::Math::Vec3D& Vector);

__NODISCARD Vector4 ToRay4(const Integrian3D::Math::Vec4F& Vector);
__NODISCARD Vector4 ToRay4(const Integrian3D::Math::Vec4D& Vector);

__NODISCARD Vector3 ToRayRotation(const Integrian3D::Math::QuatD& Quat);
