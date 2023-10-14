#pragma once

#include "EngineConstants.h"
#include "Light/Light.h"

namespace Integrian3D
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight(const Math::Vec3D& Ambient, const Math::Vec3D& Diffuse, const Math::Vec3D& Specular, const Math::Vec3D& Direction);

		virtual LightType GetLightType() const;

		__NODISCARD const Math::Vec3D& GetDirection() const;

	private:
		Math::Vec3D Direction;
	};
}