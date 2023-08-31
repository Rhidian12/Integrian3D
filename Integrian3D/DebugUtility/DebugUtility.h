#pragma once

#include "../Logger/Logger.h"

namespace Integrian3D
{
#define LOG(Category, Visibility, Format, ...) Logger::GetInstance().LogMessage(#Category, #Visibility, Format, __VA_ARGS__)

	/* ASSERT() */
#ifdef _DEBUG
#	define __BREAK() __debugbreak()

#	define __ASSERT(expr) \
		if ((expr)) {} \
		else \
		{ \
			LOG("Assert", "Error", "Assertion Triggered: %s", #expr); \
			__BREAK(); \
		}

#	define __CASSERT(expr) \
		if constexpr ((expr)) {} \
		else \
		{ \
			LOG("Assert", "Error", "Assertion Triggered: %s", #expr); \
			__BREAK(); \
		}
#else
#	define __ASSERT(expr)
#	define __CASSERT(expr)
#endif
}