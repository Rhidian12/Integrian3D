#include "Component.h"
#include "../DebugUtility/DebugUtility.h"

namespace Integrian3D
{
	Component::Component(GameObject* pOwner)
		: m_pOwner{ pOwner }
	{}

	void Component::SetOwner(GameObject* const pOwner)
	{
		if (!pOwner)
		{
			Debug::LogError("Component::SetOwner() > Passed owner is invalid", false);
			return;
		}

		m_pOwner = pOwner;
	}
}