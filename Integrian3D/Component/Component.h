#pragma once

#include "../EngineConstants.h"

namespace Integrian3D
{
	class Component
	{
	public:
		Component() = default;
		virtual ~Component() = default;

		__INLINE void SetActive(const bool isActive) { m_IsActive = isActive; }

		__NODISCARD __INLINE bool IsActive() const { return m_IsActive; }

	protected:
		bool m_IsActive;
	};
}