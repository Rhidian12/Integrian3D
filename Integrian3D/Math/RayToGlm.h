#pragma once

#include "Math/Math.h"

struct Vector2;
struct Vector3;
struct Vector4;
typedef Vector4 Quaternion;

__NODISCARD Integrian3D::Math::Vec2F ToGLM2F(const Vector2& Vector);
__NODISCARD Integrian3D::Math::Vec2D ToGLM2D(const Vector2& Vector);

__NODISCARD Integrian3D::Math::Vec3F ToGLM3F(const Vector3& Vector);
__NODISCARD Integrian3D::Math::Vec3D ToGLM3D(const Vector3& Vector);

__NODISCARD Integrian3D::Math::Vec4F ToGLM4F(const Vector4& Vector);
__NODISCARD Integrian3D::Math::Vec4D ToGLM4D(const Vector4& Vector);

__NODISCARD Integrian3D::Math::QuatD ToGLMQuat(const Quaternion& Quat);