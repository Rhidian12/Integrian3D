#pragma once

#include "../EngineConstants.h"

#include <string> /* std::string */

namespace Integrian3D
{
	class Object
	{
	public:
		void SetActive(const bool isActive);

		__NODISCARD bool IsActive() const { return m_IsActive; }

	protected:
		Object();
		explicit Object(const std::string& name);
		virtual ~Object() = default;

		std::string m_ObjectName;
		bool m_IsActive;
	};
}