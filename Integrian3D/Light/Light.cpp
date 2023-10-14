#include "Light/Light.h"

namespace Integrian3D
{
	Light::Light(const Math::Vec3D& Ambient, const Math::Vec3D& Diffuse, const Math::Vec3D& Specular)
		: Ambient{ Ambient }
		, Diffuse{ Diffuse }
		, Specular{ Specular }
	{}

	const Math::Vec3D& Light::GetAmbient() const
	{
		return Ambient;
	}

	const Math::Vec3D& Light::GetDiffuse() const
	{
		return Diffuse;
	}

	const Math::Vec3D& Light::GetSpecular() const
	{
		return Specular;
	}
}