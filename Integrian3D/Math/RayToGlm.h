#pragma once

#include "Math/Math.h"

__NODISCARD Integrian3D::Math::Vec2F ToGLM2F(const struct Vector2& Vector);
__NODISCARD Integrian3D::Math::Vec2D ToGLM2D(const struct Vector2& Vector);

__NODISCARD Integrian3D::Math::Vec3F ToGLM3F(const struct Vector3& Vector);
__NODISCARD Integrian3D::Math::Vec3D ToGLM3D(const struct Vector3& Vector);

__NODISCARD Integrian3D::Math::Vec4F ToGLM4F(const struct Vector4& Vector);
__NODISCARD Integrian3D::Math::Vec4D ToGLM4D(const struct Vector4& Vector);

__NODISCARD Integrian3D::Math::QuatD ToGLMQuat(const struct Quaternion& Quat);