#pragma once

#include "../../EngineConstants.h"
#include "../../TemplateUtils/TemplateUtils.h"

#include <string> /* std::string */
#include <type_traits> /* std::is_trivial_v, ... */

namespace Integrian3D::IO
{
	template<typename T>
	constexpr auto IsPod{ std::is_trivial_v<T> && std::is_standard_layout_v<T> };

	template<typename T>
	constexpr bool AlwaysFalse{ false };

	__NODISCARD __INLINE bool IsLittleEndian()
	{
		constexpr int n{ 1 };
		return *(char*)&n == 1;
	}

	template<typename T>
	__NODISCARD T SwapEndianness(const T val)
	{
		/* https://stackoverflow.com/questions/105252/how-do-i-convert-between-big-endian-and-little-endian-values-in-c?page=1&tab=scoredesc#tab-top */
		static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");
		static_assert(std::is_default_constructible_v<T>, "SwapEndianness<T>() > T must be default constructible");

		/* My system is Little Endian, so convert to Little Endian if target system is Big Endian */
		if (IsLittleEndian())
			return val;
		else
		{
			union ByteInfo
			{
				T Value;
				unsigned char Bytes[sizeof(T)];

				ByteInfo() { new (&Value) T{}; }
			} source, dest;

			source.Value = val;

			for (size_t i = 0; i < sizeof(T); i++)
				dest.Bytes[i] = source.Bytes[sizeof(T) - i - 1];

			return dest.Value;
		}
	}
}