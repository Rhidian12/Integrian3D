#include "Light/DirectionalLight.h"

namespace Integrian3D
{
	DirectionalLight::DirectionalLight(const Math::Vec3D& Ambient, const Math::Vec3D& Diffuse, const Math::Vec3D& Specular, const Math::Vec3D& Direction)
		: Light{ Ambient, Diffuse, Specular }
		, Direction{ Direction }
	{}

	LightType DirectionalLight::GetLightType() const
	{
		return LightType::DirectionalLight;
	}

	const Math::Vec3D& DirectionalLight::GetDirection() const
	{
		return Direction;
	}
}