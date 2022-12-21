#pragma once

#include "../../EngineConstants.h"
#include "../TimeLength.h"

namespace Integrian3D::Time
{
	/// <summary>
	/// A Timepoint is similar to std::chrono::time_point and should be viewed as it
	/// Timepoints can be initialised with a time value, received from Integrian3D::Time::Timer
	/// They can later then be used to do arithmetics to calculate time
	/// </summary>
	class Timepoint final
	{
	public:
		/// <summary>
		/// Default initializes the Timepoint to an invalid time 
		/// </summary>
		constexpr explicit Timepoint(const double time);

		/// <summary>
		/// Counts the difference between Timepoints
		/// </summary>
		template<TimeLength T>
		__NODISCARD constexpr double Count(const Timepoint& other) const
		{
			if constexpr (T == TimeLength::NanoSeconds)
				return (m_Time - other.m_Time) * NanoToSec;
			else if constexpr (T == TimeLength::MicroSeconds)
				return (m_Time - other.m_Time) * MicroToSec;
			else if constexpr (T == TimeLength::MilliSeconds)
				return (m_Time - other.m_Time) * MilliToSec;
			else if constexpr (T == TimeLength::Seconds)
				return (m_Time - other.m_Time);
			else if constexpr (T == TimeLength::Minutes)
				return (m_Time - other.m_Time) * SecToMin;
			else /* Hours */
				return (m_Time - other.m_Time) * SecToHours;
		}

		template<TimeLength T, typename Ret>
		__NODISCARD constexpr Ret Count(const Timepoint& other) const
		{
			if constexpr (T == TimeLength::NanoSeconds)
				return static_cast<Ret>((m_Time - other.m_Time) * NanoToSec);
			else if constexpr (T == TimeLength::MicroSeconds)
				return static_cast<Ret>((m_Time - other.m_Time) * MicroToSec);
			else if constexpr (T == TimeLength::MilliSeconds)
				return static_cast<Ret>((m_Time - other.m_Time) * MilliToSec);
			else if constexpr (T == TimeLength::Seconds)
				return static_cast<Ret>((m_Time - other.m_Time));
			else if constexpr (T == TimeLength::Minutes)
				return static_cast<Ret>((m_Time - other.m_Time) * SecToMin);
			else /* Hours */
				return static_cast<Ret>((m_Time - other.m_Time) * SecToHours);
		}

	private:
		double m_Time; /* Stored in seconds */
	};
}