#pragma once

#include "../../EngineConstants.h"

namespace Integrian3D
{
	class TransformComponent final
	{
	public:
		TransformComponent();

	private:
		Matrix3f Transformation;
		Point2f WorldScale;
		Point2f LocalScale;
	};
}