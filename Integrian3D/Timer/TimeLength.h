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
		Hours
	};

	inline constexpr float MilliToNano{ 1'000'000.f };
	inline constexpr float MilliToMicro{ 1000.f };
	inline constexpr float MilliToSec{ 1.f / 1'000.f };
	inline constexpr float MilliToMin{ MilliToSec * 60.f };
	inline constexpr float MilliToHours{ MilliToMin * 60.f };

	inline constexpr float NanoToMilli{ 1.f / 1'000'000.f };
	inline constexpr float MicroToMilli{ 1.f / 1000.f };
	inline constexpr float SecToMilli{ 1000.f };
	inline constexpr float MinToMilli{ SecToMilli * 60.f };
	inline constexpr float HoursToMilli{ MinToMilli * 60.f };
}