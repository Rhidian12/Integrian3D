#include "Light/DirectionalLight.h"

namespace Integrian3D
{
	void DirectionalLight::Initialize(Scene* const, const Entity, const Math::Vec3D& InAmbient, const Math::Vec3D& InDiffuse, const Math::Vec3D& InSpecular, const Math::Vec3D& InDirection)
	{
		Ambient = InAmbient;
		Diffuse = InDiffuse;
		Specular = InSpecular;
		Direction = InDirection;
	}

	LightType DirectionalLight::GetLightType() const
	{
		return LightType::DirectionalLight;
	}

	const Math::Vec3D& DirectionalLight::GetDirection() const
	{
		return Direction;
	}
}