#pragma once

#include "../EngineConstants.h"
#include "../../Array/Array.h"

#include <string_view>

namespace Integrian3D
{
	struct LoggerStatics final
	{
		void* ConsoleHandle;

		TArray<std::string_view> Categories;
		TArray<std::string_view> Visibilities;
	};
}