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
}