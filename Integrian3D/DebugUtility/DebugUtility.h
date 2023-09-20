#pragma once

#include "Logger/LogCategory.h"
#include "Logger/Logger.h"

DEFINE_LOG_CATEGORY(Assert, Integrian3D::LogVerbosity::Verbose)
DEFINE_LOG_CATEGORY(Check, Integrian3D::LogVerbosity::Verbose)
DEFINE_LOG_CATEGORY(Log, Integrian3D::LogVerbosity::Verbose);

namespace Integrian3D
{
#define LOG(Category, ErrorLevel, Format, ...) Integrian3D::Logger::GetInstance().LogMessage(#Category, ErrorLevel, Format, __VA_ARGS__)

	/* __BREAK(), __ASSERT(), __CASSERT(), __CHECK */
#ifdef _DEBUG
#	define __BREAK() __debugbreak()

#	define __ASSERT(Expr, Format, ...) \
		if ((Expr)) {} \
		else \
		{ \
			LOG(Assert, Integrian3D::LogErrorLevel::Fatal, Format, __VA_ARGS__); \
			__BREAK(); \
		}

#	define __CASSERT(Expr, Format, ...) \
		if constexpr ((Expr)) {} \
		else \
		{ \
			LOG(Assert, Integrian3D::LogErrorLevel::Fatal, Format, __VA_ARGS__); \
			__BREAK(); \
		}

#	define __CHECK(Expr) \
		if ((Expr)) {} \
		else \
		{ \
			LOG(Check, Integrian3D::LogErrorLevel::Fatal, "Check {} triggered at line {} in file {}", #Expr, __LINE__, __FILE__); \
			__BREAK(); \
		}
#else
#	define __BREAK()
#	define __ASSERT(expr, message)
#	define __CASSERT(expr, message)
#	define __CHECK(Expr)
#endif
}