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

	float ConvertMilliToSeconds(const int64 MilliSeconds);
	float ConvertMilliToMinutes(const int64 MilliSeconds);
	float ConvertMilliToHours(const int64 MilliSeconds);
}