#pragma once

#include "Array/Array.h"

#include <string_view>

namespace Integrian3D
{
	struct LoggerStatics final
	{
		constexpr LoggerStatics() = default;
		constexpr ~LoggerStatics() = default;

		TArray<std::string_view> Categories;
		TArray<std::string_view> Visibilities;
	};
}