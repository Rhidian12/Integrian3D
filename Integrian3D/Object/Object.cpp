#include "Object.h"

namespace Integrian3D
{
	Object::Object()
		: Object{ "" }
	{}

	Object::Object(const std::string& name)
		: m_ObjectName{ name }
		, m_IsActive{ true }
	{}

	void Object::SetActive(const bool isActive)
	{
		m_IsActive = isActive;
	}
}