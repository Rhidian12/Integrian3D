#pragma once

#include "Config.h"

#include "Logger/Logger.h"
#include "Macros/Macros.h"
#include "DebugUtility/DebugUtility.h"

#include <bitset>
#include <cstdint>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifdef max
#	undef max
#endif

#ifdef min
#	undef min
#endif

#ifndef MAX_COMPONENTS
#	define MAX_COMPONENTS 255
#endif

#ifndef MAX_ENTITIES
#	define MAX_ENTITIES 10'000'000
#endif

namespace Integrian3D
{
	using Entity = size_t;
	using EntitySignature = std::bitset<MAX_COMPONENTS>;

	constexpr size_t InvalidEntityID{ MAX_ENTITIES };
	constexpr size_t InvalidComponentID{ MAX_COMPONENTS };
}