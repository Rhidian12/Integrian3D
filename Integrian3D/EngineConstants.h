#pragma once

#include "Logger/Logger.h"

#include <cstdint> /* uint64_t */
#include <limits> /* std::numeric_limits */
#include <bitset> /* std::bitset */

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#undef max

namespace Integrian3D
{
	using Entity = uint32_t; /* This supports ~4 billion possible entities, seems like enough */
	using ComponentType = uint16_t; /* This supports ~65k possible component types, seems like enough */

	constexpr Entity MaxEntities{ std::numeric_limits<Entity>::max() - 1 };
	constexpr ComponentType MaxComponentTypes{ std::numeric_limits<ComponentType>::max() - 1 };

	using EntitySignature = std::bitset<MaxComponentTypes>;

	constexpr Entity InvalidEntityID{ std::numeric_limits<Entity>::max() };
	constexpr ComponentType InvalidComponentID{ std::numeric_limits<ComponentType>::max() };

	/* [[nodiscard]] */
#define __NODISCARD [[nodiscard]]

	/* inline */
#ifdef _WIN32
#define __INLINE __forceinline
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
#endif

	/* std::move */
#ifdef _DEBUG
#define __MOVE(type, val) static_cast<type&&>(val)
#else
#define __MOVE(type, val) std::move(val)
#endif

	/* std::forward */
#ifdef _DEBUG
#define __FORWARD(type, val) static_cast<type&&>(val)
#else
#define __FORWARD(type, val) std::forward<type>(val)
#endif
}