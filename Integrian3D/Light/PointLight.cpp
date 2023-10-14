#include "Light/PointLight.h"

namespace Integrian3D
{
	PointLight::PointLight(const Math::Vec3D& Ambient, const Math::Vec3D& Diffuse, const Math::Vec3D& Specular)
		: Light(Ambient, Diffuse, Specular)
	{}
}