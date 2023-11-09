#pragma once

#include "EngineConstants.h"
#include "Timer/TimeLength.h"

namespace Integrian3D::Time
{
	namespace Detail
	{
		struct _Milliseconds
		{
			int64 MS;
		};
	}

	// uint64 is required for for literal operator
	constexpr Detail::_Milliseconds operator""_ms(const uint64 i)
	{
		return Detail::_Milliseconds{ static_cast<int64>(i) };
	}

	/// <summary>
	/// A Timepoint is similar to std::chrono::time_point and should be viewed as it
	/// Timepoints can be initialised with a time value, received from Integrian3D::Time::Timer
	/// They can later then be used to do arithmetics to calculate time
	/// </summary>
	class Timepoint final
	{
	public:
		constexpr Timepoint()
			: MilliSeconds{}
		{}
		constexpr explicit Timepoint(const Detail::_Milliseconds time)
			: MilliSeconds{ time.MS }
		{}
		constexpr ~Timepoint() = default;

		constexpr Timepoint(const Timepoint&) noexcept = default;
		constexpr Timepoint(Timepoint&&) noexcept = default;
		constexpr Timepoint& operator=(const Timepoint&) noexcept = default;
		constexpr Timepoint& operator=(Timepoint&&) noexcept = default;

		template<TimeLength T = TimeLength::MilliSeconds>
		__NODISCARD constexpr int64 Count() const
		{
			if constexpr (T == TimeLength::NanoSeconds)
				return MilliSeconds * 1'000'000;
			else if constexpr (T == TimeLength::MicroSeconds)
				return MilliSeconds * 1000;
			else if constexpr (T == TimeLength::MilliSeconds)
				return MilliSeconds;
			else if constexpr (T == TimeLength::Seconds)
				return MilliSeconds / 1000;
			else if constexpr (T == TimeLength::Minutes)
				return MilliSeconds / 1000 / 60;
			else /* Hours */
				return MilliSeconds / 1000 / 60 / 60;
		}

#pragma region Operators
		friend constexpr Timepoint operator-(const Timepoint& a, const Timepoint& b);
		friend constexpr Timepoint operator+(const Timepoint& a, const Timepoint& b);

		friend constexpr Timepoint& operator+=(Timepoint& a, const Timepoint& b);
		friend constexpr Timepoint& operator-=(Timepoint& a, const Timepoint& b);

		friend constexpr bool operator==(const Timepoint& a, const Timepoint& b);
		friend constexpr bool operator!=(const Timepoint& a, const Timepoint& b);

		friend constexpr bool operator<(const Timepoint& a, const Timepoint& b);
		friend constexpr bool operator<=(const Timepoint& a, const Timepoint& b);
		friend constexpr bool operator>(const Timepoint& a, const Timepoint& b);
		friend constexpr bool operator>=(const Timepoint& a, const Timepoint& b);
#pragma endregion

	private:
		int64 MilliSeconds;
	};

#pragma region Operators
	constexpr Timepoint operator-(const Timepoint& a, const Timepoint& b)
	{
		return Timepoint{ Detail::_Milliseconds{ a.MilliSeconds - b.MilliSeconds } };
	}
	constexpr Timepoint operator+(const Timepoint& a, const Timepoint& b)
	{
		return Timepoint{ Detail::_Milliseconds{ a.MilliSeconds + b.MilliSeconds } };
	}
	constexpr Timepoint& operator+=(Timepoint& a, const Timepoint& b)
	{
		a.MilliSeconds += b.MilliSeconds;

		return a;
	}
	constexpr Timepoint& operator-=(Timepoint& a, const Timepoint& b)
	{
		a.MilliSeconds -= b.MilliSeconds;

		return a;
	}
	constexpr bool operator==(const Timepoint& a, const Timepoint& b)
	{
		return a.MilliSeconds == b.MilliSeconds;
	}
	constexpr bool operator!=(const Timepoint& a, const Timepoint& b)
	{
		return !(a == b);
	}
	constexpr bool operator<(const Timepoint& a, const Timepoint& b)
	{
		return a.MilliSeconds < b.MilliSeconds;
	}
	constexpr bool operator<=(const Timepoint& a, const Timepoint& b)
	{
		return a.MilliSeconds <= b.MilliSeconds;
	}
	constexpr bool operator>(const Timepoint& a, const Timepoint& b)
	{
		return a.MilliSeconds > b.MilliSeconds;
	}
	constexpr bool operator>=(const Timepoint& a, const Timepoint& b)
	{
		return a.MilliSeconds >= b.MilliSeconds;
	}

#pragma endregion
}