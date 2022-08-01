#pragma once

#include <iostream> /* std::cout for users */

#include "../Logger/Logger.h"

namespace Integrian3D
{
	namespace Debug
	{
		/* [CRINGE]: These macros will only work on MSVC (__FILE__, __LINE__) */
#define LogMessage(message, bVerbose) Logger::GetInstance().LogMessage((message), __LINE__, __FILE__, (bVerbose))
#define LogWarning(message, bVerbose) Logger::GetInstance().LogWarning((message), __LINE__, __FILE__, (bVerbose))
#define LogError(message, bVerbose) Logger::GetInstance().LogError((message), __LINE__, __FILE__, (bVerbose))
#define LogCustomMessage(message, colour, bVerbose) Logger::GetInstance().LogCustomMessage((message), __LINE__, __FILE__, (colour), (bVerbose))
	}
}