#pragma once

#include "../../EngineConstants.h"

#include "../../Component/Component.h"

namespace Integrian3D
{
	class TransformComponent;

	class TestRotateComponent final : public Component
	{
	public:
		void Rotate(TransformComponent& transform);
	};
}