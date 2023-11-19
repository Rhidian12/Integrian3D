#include "Light/Light.h"

namespace Integrian3D
{
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