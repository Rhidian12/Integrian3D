#include "Math/RayToGlm.h"

#include <raylib.h>

Integrian3D::Math::Vec2F ToGLM2F(const Vector2& Vector)
{
	return Integrian3D::Math::Vec2F{ Vector.x, Vector.y };
}

Integrian3D::Math::Vec2D ToGLM2D(const Vector2& Vector)
{
	return Integrian3D::Math::Vec2D{ static_cast<double>(Vector.x), static_cast<double>(Vector.y) };
}

Integrian3D::Math::Vec3F ToGLM3F(const Vector3& Vector)
{
	return Integrian3D::Math::Vec3F{ Vector.x, Vector.y, Vector.z };
}

Integrian3D::Math::Vec3D ToGLM3D(const Vector3& Vector)
{
	return Integrian3D::Math::Vec3D{ static_cast<double>(Vector.x), static_cast<double>(Vector.y), static_cast<double>(Vector.z) };
}

Integrian3D::Math::Vec4F ToGLM4F(const Vector4& Vector)
{
	return Integrian3D::Math::Vec4F{ Vector.x, Vector.y, Vector.z, Vector.w };
}

Integrian3D::Math::Vec4D ToGLM4D(const Vector4& Vector)
{
	return Integrian3D::Math::Vec4D{ static_cast<double>(Vector.x), static_cast<double>(Vector.y), static_cast<double>(Vector.z), static_cast<double>(Vector.w) };
}

Integrian3D::Math::QuatD ToGLMQuat(const Quaternion& Quat)
{
	return Integrian3D::Math::QuatD{ static_cast<double>(Quat.x), static_cast<double>(Quat.y), static_cast<double>(Quat.z), static_cast<double>(Quat.w) };
}