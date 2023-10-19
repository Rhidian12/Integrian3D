#pragma once

#include "EngineConstants.h"

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

	inline constexpr float SecToNano{ 1'000'000'000.f };
	inline constexpr float SecToMicro{ 1'000'000.f };
	inline constexpr float SecToMilli{ 1'000.f };
	inline constexpr float SecToMin{ 1.f / 60.f };
	inline constexpr float SecToHours{ 1.f / 3600.f };

	inline constexpr float NanoToSec{ 1.f / 1'000'000'000.f };
	inline constexpr float MicroToSec{ 1.f / 1'000'000.f };
	inline constexpr float MilliToSec{ 1.f / 1'000.f };
	inline constexpr float MinToSec{ 60.f };
	inline constexpr float HoursToSec{ 3600.f };
}