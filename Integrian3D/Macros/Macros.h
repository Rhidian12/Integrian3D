#pragma once

#undef max
#undef min

namespace Integrian3D
{
	/* [[nodiscard]] */
#define __NODISCARD [[nodiscard]]

	/* inline */
#ifdef _WIN32
#define __INLINE __forceinline
#elif
#define __INLINE inline
#endif

	/* RNG seed */
#ifndef SEED
#define SEED rand() % RAND_MAX
#endif

	/* ASSERT() */
#ifdef _DEBUG
#define __BREAK() __debugbreak()
#define __ASSERT(expr) \
	if ((expr)) {} \
	else \
	{ \
		Debug::Logger::GetInstance().LogAssertion(#expr, __LINE__, __FILE__, Debug::MessageColour::Yellow, true); \
		__BREAK(); \
	}
#define __CASSERT(expr) \
	if constexpr ((expr)) {} \
	else \
	{ \
		Debug::Logger::GetInstance().LogAssertion(#expr, __LINE__, __FILE__, Debug::MessageColour::Yellow, true); \
		__BREAK(); \
	}
#else
#define __ASSERT(expr)
#define __CASSERT(expr)
#endif

	/* std::move */
#ifdef _DEBUG
#define __MOVE(val) static_cast<std::remove_reference_t<decltype((val))>&&>((val))
#else
#define __MOVE(val) std::move((val))
#endif

	/* std::forward */
#ifdef _DEBUG
#define __FORWARD(val) static_cast<decltype((val))&&>((val))
#else
#define __FORWARD(val) std::forward<decltype((val))>((val))
#endif

#define __DELETE(p) do { delete p; p = nullptr; } while (0);
#define __DELETEARR(p) do { delete[] (p); (p) = nullptr; } while (0);
}