#include "Timer/TimeLength.h"

namespace Integrian3D::Time
{
	float ConvertMilliToSeconds(int64 MilliSeconds)
	{
		return static_cast<float>(MilliSeconds) / 1000.f;
	}

	float ConvertMilliToMinutes(const int64 MilliSeconds)
	{
		return ConvertMilliToSeconds(MilliSeconds) / 60.f;
	}

	float ConvertMilliToHours(const int64 MilliSeconds)
	{
		return ConvertMilliToMinutes(MilliSeconds) / 60.f;
	}
}