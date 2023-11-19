#pragma once

#include "EngineConstants.h"
#include "Component/Component.h"
#include "Math/Math.h"

namespace Integrian3D
{
	enum class LightType : int32
	{
		PointLight,
		DirectionalLight
	};

	class Light : public Component
	{
	public:
		virtual ~Light() = default;

		virtual LightType GetLightType() const = 0;

		const Math::Vec3D& GetAmbient() const;
		const Math::Vec3D& GetDiffuse() const;
		const Math::Vec3D& GetSpecular() const;

	protected:
		Math::Vec3D Ambient;
		Math::Vec3D Diffuse;
		Math::Vec3D Specular;
	};
}