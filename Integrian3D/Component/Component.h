#pragma once

#include "../EngineConstants.h"

namespace Integrian3D
{
	class Component
	{
	public:
		explicit Component(class GameObject* pOwner);
		virtual ~Component() = default;

		virtual void Update() {}
		virtual void Render() const {}

	protected:
		class GameObject* m_pOwner;
	};
}