#pragma once

#include "../../EngineConstants.h"

namespace Integrian3D
{
	class TransformComponent final
	{
	public:
		TransformComponent();

	private:
		Matrix4f Transformation;

		Point3f WorldScale;
		Point3f LocalScale;

		Point3f WorldAngle; /* In Radians */
		Point3f LocalAngle; /* In Radians */

		bool bShouldRecalculateTransform;
	};
}