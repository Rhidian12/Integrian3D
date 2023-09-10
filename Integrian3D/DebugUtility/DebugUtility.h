#pragma once

#include "Logger/Logger.h"

namespace Integrian3D
{
#define LOG(Category, Visibility, Format, ...)	static_assert(Integrian3D::CheckLogCategory(#Category), "Log Category has not been defined"); \
												Logger::GetInstance().LogMessage(#Category, #Visibility, Format, __VA_ARGS__)

	/* __ASSERT(), __CASSERT(), __CHECK */
#ifdef _DEBUG
#	define __BREAK() __debugbreak()

#	define __ASSERT(Expr, Format, ...) \
		if ((Expr)) {} \
		else \
		{ \
			LOG("Assert", "Fatal", Format, __VA_ARGS__); \
			__BREAK(); \
		}

#	define __CASSERT(Expr, Format, ...) \
		if constexpr ((Expr)) {} \
		else \
		{ \
			LOG("Assert", "Fatal", Format, __VA_ARGS__); \
			__BREAK(); \
		}

#	define __CHECK(Expr) \
		if ((Expr)) {} \
		else \
		{ \
			LOG("Check", "Fatal", "Check %s triggered at line %i in file %s", #Expr, __LINE__, __FILE__); \
			__BREAK(); \
		}
#else
#	define __BREAK()
#	define __ASSERT(expr, message)
#	define __CASSERT(expr, message)
#endif
}