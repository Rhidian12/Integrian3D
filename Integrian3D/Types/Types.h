#pragma once

#include "Macros/Macros.h"

namespace Detail
{
	template<typename Type32Bits, typename Type64Bits, int PointerSize>
	struct SelectPointerType final {};

	template<typename Type32Bits, typename Type64Bits>
	struct SelectPointerType<Type32Bits, Type64Bits, 4>
	{
		using PointerType = Type32Bits;
	};

	template<typename Type32Bits, typename Type64Bits>
	struct SelectPointerType<Type32Bits, Type64Bits, 8>
	{
		using PointerType = Type64Bits;
	};
}

struct GenericTypes
{
	// unsigned types
	
	// 8-bit unsigned integer
	using uint8 = unsigned char;

	// 16-bit unsigned integer
	using uint16 = unsigned short int;

	// 32-bit unsigned integer
	using uint32 = unsigned int;

	// 64-bit unsigned integer
	using uint64 = unsigned long long;

	// signed types
	
	// 8-bit signed integer
	using int8 = signed char;

	// 16-bit signed integer
	using int16 = signed short int;

	// 32-bit signed integer
	using int32 = signed int;

	// 64-bit signed integer
	using int64 = signed long long;

	// pointer types
	using SIZE_T = Detail::SelectPointerType<uint32, uint64, sizeof(void*)>::PointerType;
	using SSIZE_T = Detail::SelectPointerType<int32, int64, sizeof(void*)>::PointerType;
};

#ifdef _WIN32
struct WindowsTypes final : public GenericTypes
{
	using SIZE_T = unsigned __int64;
	using SSIZE_T = __int64;

	using DWORD = unsigned long;
};

using PlatformTypes = WindowsTypes;

#else
#error Only Windows is supported
#endif

// unsigned types

// 8-bit unsigned integer
using uint8 = PlatformTypes::uint8;

// 16-bit unsigned integer
using uint16 = PlatformTypes::uint16;

// 32-bit unsigned integer
using uint32 = PlatformTypes::uint32;

// 64-bit unsigned integer
using uint64 = PlatformTypes::uint64;

// signed types

// 8-bit signed integer
using int8 = PlatformTypes::int8;

// 16-bit signed integer
using int16 = PlatformTypes::int16;

// 32-bit signed integer
using int32 = PlatformTypes::int32;

// 64-bit signed integer
using int64 = PlatformTypes::int64;

// pointer types
using SIZE_T = PlatformTypes::SIZE_T;
using SSIZE_T = PlatformTypes::SSIZE_T;

using DWORD = PlatformTypes::DWORD;

namespace Integrian3D
{
	struct Size_P final
	{
		constexpr explicit Size_P(const uint64 n)
			: _Size{ n }
		{}

		uint64 _Size;
	};
	struct Capacity_P final
	{
		constexpr explicit Capacity_P(const uint64 n)
			: _Capacity{ n }
		{}

		uint64 _Capacity;
	};

	__NODISCARD __INLINE constexpr Size_P operator""_size(const uint64 i)
	{
		return Size_P{ i };
	}
	__NODISCARD __INLINE constexpr Capacity_P operator""_capacity(const uint64 i)
	{
		return Capacity_P{ i };
	}
}