#pragma once

#include "Macros.h"

#include <type_traits>

namespace Integrian3D::TUtils
{
	#ifdef _MSC_VER
	#define __FUNCTION_NAME__ __FUNCSIG__
	#else
	#define __FUNCTION_NAME__ __PRETTY_FUNCTION__
	#endif

	template<typename ...>
	using Void = void;

	template<typename T>
	constexpr bool AlwaysFalse{ false };

	template<typename From, typename To>
	constexpr bool IsExplicitlyConvertible{ std::is_constructible_v<To, From> && std::is_convertible_v<From, To> };
}