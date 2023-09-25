#include "Math/GlmToRay.h"

Vector2 ToRay2(const Integrian3D::Math::Vec2F& Vector)
{
	return Vector2{ Vector.x, Vector.y };
}

Vector2 ToRay2(const Integrian3D::Math::Vec2D& Vector)
{
	return Vector2{ static_cast<float>(Vector.x), static_cast<float>(Vector.y) };
}

Vector3 ToRay3(const Integrian3D::Math::Vec3F& Vector)
{
	return Vector3{ Vector.x, Vector.y, Vector.z };
}

Vector3 ToRay3(const Integrian3D::Math::Vec3D& Vector)
{
	return Vector3{ static_cast<float>(Vector.x), static_cast<float>(Vector.y), static_cast<float>(Vector.z) };
}

Vector4 ToRay4(const Integrian3D::Math::Vec4F& Vector)
{
	return Vector4{ Vector.x, Vector.y, Vector.z, Vector.w };
}

Vector4 ToRay4(const Integrian3D::Math::Vec4D& Vector)
{
	return Vector4{ static_cast<float>(Vector.x), static_cast<float>(Vector.y), static_cast<float>(Vector.z), static_cast<float>(Vector.w) };
}

Vector3 ToRayRotation(const Integrian3D::Math::QuatD& Quat)
{
	Integrian3D::Math::Vec3D Vector{ glm::eulerAngles(glm::normalize(Quat)) };

	Vector.x = Integrian3D::Math::ToDegrees(Vector.x);
	Vector.y = Integrian3D::Math::ToDegrees(Vector.y);
	Vector.z = Integrian3D::Math::ToDegrees(Vector.z);

	return ToRay3(Vector);
}
