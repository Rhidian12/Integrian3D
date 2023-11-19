#pragma once

#include "EngineConstants.h"
#include "Light/Light.h"

namespace Integrian3D
{
	class Scene;

	class PointLight : public Light
	{
	public:
		void Initialize(Scene* const Scene, const Entity Entity, const Math::Vec3D& InAmbient, const Math::Vec3D& InDiffuse,
			const Math::Vec3D& InSpecular, const float InMaxRadius, const float InIntensity);

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