#pragma once

#include "EngineConstants.h"
#include "Light/Light.h"

namespace Integrian3D
{
	class PointLight : public Light
	{
	public:
		PointLight(const Math::Vec3D& Ambient, const Math::Vec3D& Diffuse, const Math::Vec3D& Specular,
			const float MaxRadius, const float Intensity);

		void SetMaxRadius(const float InMaxRadius);
		void SetIntensity(const float InIntensity);

		__NODISCARD virtual LightType GetLightType() const;
		__NODISCARD float GetMaxRadius() const;
		__NODISCARD float GetIntensity() const;

	private:
		float MaxRadius;
		float Intensity;
	};
}