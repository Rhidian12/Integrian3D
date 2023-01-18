#pragma once

#include "../EngineConstants.h"

#include <string> /* std::string */

namespace Integrian3D
{
	class Object
	{
	protected:
		Object();
		explicit Object(const std::string& name);
		virtual ~Object() = default;

		std::string m_ObjectName;
	};
}