#pragma once

#include "../../EngineConstants.h"
#include "../../Component/Component.h"

namespace Integrian3D
{
	class TestRotateComponent final : public Component
	{
	public:
		explicit TestRotateComponent(GameObject* pOwner);

		virtual void Update() override;
	};
}