#pragma once

#include "EngineConstants.h"
#include "Component/Component.h"
#include "Math/Math.h"

namespace Integrian3D
{
	class Light : public Component
	{
	public:
		Light(const Math::Vec3D& Ambient, const Math::Vec3D& Diffuse, const Math::Vec3D& Specular);
		virtual ~Light() = default;

		const Math::Vec3D& GetAmbient() const;
		const Math::Vec3D& GetDiffuse() const;
		const Math::Vec3D& GetSpecular() const;

	protected:
		Math::Vec3D Ambient;
		Math::Vec3D Diffuse;
		Math::Vec3D Specular;
	};
}