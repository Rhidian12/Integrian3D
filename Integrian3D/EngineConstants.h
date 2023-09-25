#pragma once

#include "Config.h"

#include "Logger/Logger.h"
#include "Macros/Macros.h"
#include "DebugUtility/DebugUtility.h"
#include "Types/Types.h"

#include <bitset>

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
	using Entity = SIZE_T;
	using EntitySignature = std::bitset<MAX_COMPONENTS>;

	constexpr SIZE_T InvalidEntityID{ MAX_ENTITIES };
	constexpr SIZE_T InvalidComponentID{ MAX_COMPONENTS };
}