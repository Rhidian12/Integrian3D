#pragma once

#include "EngineConstants.h"
#include "Light/Light.h"

namespace Integrian3D
{
	class PointLight : public Light
	{
	public:
		PointLight(const Math::Vec3D& Ambient, const Math::Vec3D& Diffuse, const Math::Vec3D& Specular);

		virtual LightType GetLightType() const;
	};
}