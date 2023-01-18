#pragma once

#include "../EngineConstants.h"
#include "../GameObject/GameObject.h"

namespace Integrian3D
{
	class Component : public Object
	{
	public:
		explicit Component(class GameObject* pOwner);
		virtual ~Component() = default;

		virtual void Start() {}
		virtual void Update() {}
		virtual void Render() const {}

		void SetOwner(class GameObject* const pOwner);

		__NODISCARD class GameObject* const GetOwner() const { return m_pOwner; }

	protected:
		class GameObject* m_pOwner;
	};
}