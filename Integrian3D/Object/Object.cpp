#include "Object.h"

namespace Integrian3D
{
	Object::Object()
		: m_ObjectName{}
	{}

	Object::Object(const std::string& name)
		: m_ObjectName{ name }
	{}
}