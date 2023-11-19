#pragma once

#include "EngineConstants.h"
#include "Light/Light.h"

namespace Integrian3D
{
	class Scene;

	class DirectionalLight : public Light
	{
	public:
		void Initialize(Scene* const Scene, const Entity Entity, const Math::Vec3D& InAmbient, const Math::Vec3D& InDiffuse, const Math::Vec3D& InSpecular, const Math::Vec3D& InDirection);

		virtual LightType GetLightType() const;

		__NODISCARD const Math::Vec3D& GetDirection() const;

	private:
		Math::Vec3D Direction;
	};
}