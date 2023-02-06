#pragma once

#include "../EngineConstants.h"

namespace Integrian3D::TUtils
{
#ifdef _MSC_VER
#define __FUNCTION_NAME__ __FUNCSIG__
#elif
#define __FUNCTION_NAME__ __PRETTY_FUNCTION__
#endif

	template<typename ...>
	using Void = void;

	template<typename T>
	constexpr bool AlwaysFalse{ false };

	template<typename From, typename To>
	constexpr bool IsExplicitlyConvertible{ std::is_constructible_v<To, From> && std::is_convertible_v<From, To> };
}