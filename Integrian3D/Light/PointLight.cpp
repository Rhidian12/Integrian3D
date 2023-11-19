#include "Light/PointLight.h"

namespace Integrian3D
{
	void PointLight::Initialize(Scene* const, const Entity, const Math::Vec3D& InAmbient, const Math::Vec3D& InDiffuse, const Math::Vec3D& InSpecular, const float InMaxRadius, const float InIntensity)
	{
		Ambient = InAmbient;
		Diffuse = InDiffuse;
		Specular = InSpecular;
		MaxRadius = InMaxRadius;
		Intensity = InIntensity;
	}

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