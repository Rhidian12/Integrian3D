#include "Light/PointLight.h"

namespace Integrian3D
{
	PointLight::PointLight(const Math::Vec3D& Ambient, const Math::Vec3D& Diffuse, const Math::Vec3D& Specular,
		const float MaxRadius, const float Intensity)
		: Light(Ambient, Diffuse, Specular)
		, MaxRadius{ MaxRadius }
		, Intensity{ Intensity }
	{}

	void PointLight::SetMaxRadius(const float InMaxRadius)
	{
		MaxRadius = InMaxRadius;
	}

	void PointLight::SetIntensity(const float InIntensity)
	{
		Intensity = InIntensity;
	}

	LightType PointLight::GetLightType() const
	{
		return LightType::PointLight;
	}

	float PointLight::GetMaxRadius() const
	{
		return MaxRadius;
	}

	float PointLight::GetIntensity() const
	{
		return Intensity;
	}
}