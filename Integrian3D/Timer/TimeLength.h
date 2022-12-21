#pragma once

#include "../EngineConstants.h"

namespace Integrian3D
{
	enum class TimeLength
	{
		NanoSeconds,
		MicroSeconds,
		MilliSeconds,
		Seconds,
		Minutes,
		Hours,
	};

	inline constexpr double NanoToSec{ 216'000.0 };
	inline constexpr double MicroToSec{ 3600.0 };
	inline constexpr double MilliToSec{ 60.0 };
	inline constexpr double SecToMin{ 1.0 / 60.0 };
	inline constexpr double SecToHours{ 1.0 / 3600.0 };
}