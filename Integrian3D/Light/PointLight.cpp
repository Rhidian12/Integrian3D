#include "Light/PointLight.h"

namespace Integrian3D
{
	PointLight::PointLight(const Math::Vec3D& Ambient, const Math::Vec3D& Diffuse, const Math::Vec3D& Specular, const float MaxRadius)
		: Light(Ambient, Diffuse, Specular)
		, MaxRadius{ MaxRadius }
	{}

	LightType PointLight::GetLightType() const
	{
		return LightType::PointLight;
	}

	float PointLight::GetMaxRadius() const
	{
		return MaxRadius;
	}
}