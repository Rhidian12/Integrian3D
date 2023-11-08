#pragma once

#include "Logger/LogCategory.h"
#include "Logger/Logger.h"

DEFINE_LOG_CATEGORY(Assert, Integrian3D::LogVerbosity::Verbose)
DEFINE_LOG_CATEGORY(Check, Integrian3D::LogVerbosity::Verbose)
DEFINE_LOG_CATEGORY(Log, Integrian3D::LogVerbosity::Verbose);

namespace Integrian3D
{
#define LOG(Category, ErrorLevel, Format, ...) Integrian3D::Logger::GetInstance().LogMessage(#Category, ErrorLevel, Format, __VA_ARGS__)

	/* __BREAK(), __ASSERT(), __CASSERT() */
#if _DEBUG
#	define __BREAK() __debugbreak()

#	define __ASSERT(Expr, Format, ...) \
		if ((Expr)) {} \
		else \
		{ \
			LOG(Assert, Integrian3D::LogErrorLevel::Fatal, Format, __VA_ARGS__); \
		}

#	define IASSERT_MSG(Expr, Format, ...) \
		if ((Expr)) {} \
		else \
		{ \
			LOG(Assert, Integrian3D::LogErrorLevel::Fatal, Format, __VA_ARGS__); \
		}

#	define IASSERT(Expr) \
		if ((Expr)) {} \
		else \
		{ \
			LOG(Assert, Integrian3D::LogErrorLevel::Fatal, "Assert {} triggered at line {} in file {}", #Expr, __LINE__, __FILE__); \
		}

#	define __CASSERT(Expr, Format, ...) \
		if constexpr ((Expr)) {} \
		else \
		{ \
			LOG(Assert, Integrian3D::LogErrorLevel::Fatal, Format, __VA_ARGS__); \
		}
#else
#	define __BREAK()
#	define __ASSERT(Expr, Format, ...)
#	define IASSERT_MSG(Expr, Format, ...)
#	define IASSERT(Expr)
#	define __CASSERT(Expr, Format, ...)
#endif

	// __CHECK and ICHECK_MSG are very similar to asserts, but do not intentionally crash the program, and are also NOT compiled out in Release
#	define __CHECK(Expr) \
		if ((Expr)) {} \
		else \
		{ \
			LOG(Check, Integrian3D::LogErrorLevel::Error, "Check {} triggered at line {} in file {}", #Expr, __LINE__, __FILE__); \
		}

#	define ICHECK_MSG(Expr, Format, ...) \
		if (Expr) {} \
		else \
		{ \
			LOG(Check, Integrian3D::LogErrorLevel::Error, Format, __VA_ARGS__); \
		}
}