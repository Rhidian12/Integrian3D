#pragma once

#include "../EngineConstants.h"

namespace Integrian3D::Time
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

	inline constexpr double SecToNano{ 1'000'000'000.0 };
	inline constexpr double SecToMicro{ 1'000'000.0 };
	inline constexpr double SecToMilli{ 1'000.0 };
	inline constexpr double SecToMin{ 1.0 / 60.0 };
	inline constexpr double SecToHours{ 1.0 / 3600.0 };
}