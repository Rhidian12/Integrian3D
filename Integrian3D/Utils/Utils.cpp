#include "Utils.h"

namespace Integrian3D
{
	namespace Utils
	{
		constexpr size_t ConstexprStringHash(const char* pString)
		{
			return CRC32<sizeof(pString) - 2>(pString) ^ 0xFFFFFFFF;
		}
	}
}