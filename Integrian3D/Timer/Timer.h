#pragma once

#include "../EngineConstants.h"
#include "TimeLength.h"
#include "Timepoint/Timepoint.h"

#include <memory> /* std::unique_ptr */

namespace Integrian3D::Time
{
	class Timer final
	{
	public:
		constexpr ~Timer() = default;

		static Timer& GetInstance();

		Timer(const Timer&) noexcept = delete;
		Timer(Timer&&) noexcept = delete;
		Timer& operator=(const Timer&) noexcept = delete;
		Timer& operator=(Timer&&) noexcept = delete;

		constexpr void Start()
		{
			m_PreviousTimepoint = Now();
		}
		constexpr void Update()
		{
			m_StartTimepoint = Now();

			m_ElapsedSeconds = m_StartTimepoint.Count<TimeLength::Seconds>(m_PreviousTimepoint);
			m_ElapsedSeconds = std::min(m_ElapsedSeconds, m_MaxElapsedSeconds);

			m_TotalElapsedSeconds += m_ElapsedSeconds;

			m_PreviousTimepoint = m_StartTimepoint;

			m_FPS = static_cast<int>(1.0 / m_ElapsedSeconds);
		}

		__NODISCARD constexpr Timepoint Now() const { return Timepoint{ GetElapsedSeconds() }; }
		__NODISCARD constexpr double GetElapsedSeconds() const { return m_ElapsedSeconds; }
		__NODISCARD constexpr double GetFixedElapsedSeconds() const { return m_TimePerFrame; }
		__NODISCARD constexpr double GetTotalElapsedSeconds() const { return m_TotalElapsedSeconds; }
		__NODISCARD constexpr int GetFPS() const { return m_FPS; }
		__NODISCARD constexpr double GetTimePerFrame() const { return m_TimePerFrame; }

#pragma region GetElapsedTime
		template<TimeLength T>
		__NODISCARD constexpr double GetElapsedTime() const
		{
			if constexpr (T == TimeLength::NanoSeconds)
				return m_ElapsedSeconds * NanoToSec;
			else if constexpr (T == TimeLength::MicroSeconds)
				return m_ElapsedSeconds * MicroToSec;
			else if constexpr (T == TimeLength::MilliSeconds)
				return m_ElapsedSeconds * MilliToSec;
			else if constexpr (T == TimeLength::Seconds)
				return m_ElapsedSeconds;
			else if constexpr (T == TimeLength::Minutes)
				return m_ElapsedSeconds * SecToMin;
			else /* Hours */
				return m_ElapsedSeconds * SecToHours;
		}
		template<TimeLength T, typename Ret>
		__NODISCARD constexpr Ret GetElapsedTime() const
		{
			if constexpr (T == TimeLength::NanoSeconds)
				return static_cast<Ret>(m_ElapsedSeconds * NanoToSec);
			else if constexpr (T == TimeLength::MicroSeconds)
				return static_cast<Ret>(m_ElapsedSeconds * MicroToSec);
			else if constexpr (T == TimeLength::MilliSeconds)
				return static_cast<Ret>(m_ElapsedSeconds * MilliToSec);
			else if constexpr (T == TimeLength::Seconds)
				return static_cast<Ret>(m_ElapsedSeconds);
			else if constexpr (T == TimeLength::Minutes)
				return static_cast<Ret>(m_ElapsedSeconds * SecToMin);
			else /* Hours */
				return static_cast<Ret>(m_ElapsedSeconds * SecToHours);
		}
#pragma endregion

#pragma region GetFixedElapsedTime
		template<TimeLength T>
		__NODISCARD constexpr double GetFixedElapsedTime() const
		{
			if constexpr (T == TimeLength::NanoSeconds)
				return m_TimePerFrame * NanoToSec;
			else if constexpr (T == TimeLength::MicroSeconds)
				return m_TimePerFrame * MicroToSec;
			else if constexpr (T == TimeLength::MilliSeconds)
				return m_TimePerFrame * MilliToSec;
			else if constexpr (T == TimeLength::Seconds)
				return m_TimePerFrame;
			else if constexpr (T == TimeLength::Minutes)
				return m_TimePerFrame * SecToMin;
			else /* Hours */
				return m_TimePerFrame * SecToHours;
		}
		template<TimeLength T, typename Ret>
		__NODISCARD constexpr Ret GetFixedElapsedTime() const
		{
			if constexpr (T == TimeLength::NanoSeconds)
				return static_cast<Ret>(m_TimePerFrame * NanoToSec);
			else if constexpr (T == TimeLength::MicroSeconds)
				return static_cast<Ret>(m_TimePerFrame * MicroToSec);
			else if constexpr (T == TimeLength::MilliSeconds)
				return static_cast<Ret>(m_TimePerFrame * MilliToSec);
			else if constexpr (T == TimeLength::Seconds)
				return static_cast<Ret>(m_TimePerFrame);
			else if constexpr (T == TimeLength::Minutes)
				return static_cast<Ret>(m_TimePerFrame * SecToMin);
			else /* Hours */
				return static_cast<Ret>(m_TimePerFrame * SecToHours);
		}
#pragma endregion

#pragma region GetTotalElapsedTime
		template<TimeLength T>
		__NODISCARD constexpr double GetTotalElapsedTime() const
		{
			if constexpr (T == TimeLength::NanoSeconds)
				return m_TotalElapsedSeconds * NanoToSec;
			else if constexpr (T == TimeLength::MicroSeconds)
				return m_TotalElapsedSeconds * MicroToSec;
			else if constexpr (T == TimeLength::MilliSeconds)
				return m_TotalElapsedSeconds * MilliToSec;
			else if constexpr (T == TimeLength::Seconds)
				return m_TotalElapsedSeconds;
			else if constexpr (T == TimeLength::Minutes)
				return m_TotalElapsedSeconds * SecToMin;
			else /* Hours */
				return m_TotalElapsedSeconds * SecToHours;
		}
		template<TimeLength T, typename Ret>
		__NODISCARD constexpr Ret GetTotalElapsedTime() const
		{
			if constexpr (T == TimeLength::NanoSeconds)
				return static_cast<Ret>(m_TotalElapsedSeconds * NanoToSec);
			else if constexpr (T == TimeLength::MicroSeconds)
				return static_cast<Ret>(m_TotalElapsedSeconds * MicroToSec);
			else if constexpr (T == TimeLength::MilliSeconds)
				return static_cast<Ret>(m_TotalElapsedSeconds * MilliToSec);
			else if constexpr (T == TimeLength::Seconds)
				return static_cast<Ret>(m_TotalElapsedSeconds);
			else if constexpr (T == TimeLength::Minutes)
				return static_cast<Ret>(m_TotalElapsedSeconds * SecToMin);
			else /* Hours */
				return static_cast<Ret>(m_TotalElapsedSeconds * SecToHours);
		}
#pragma endregion

	private:
		constexpr Timer()
			: m_MaxElapsedSeconds{ 0.1 }
			, m_ElapsedSeconds{}
			, m_TotalElapsedSeconds{}
			, m_FPS{}
			, m_FPSCounter{}
			, m_FPSTimer{}
			, m_TimePerFrame{ 1.0 / 60.0 }
			, m_StartTimepoint{}
			, m_PreviousTimepoint{}
		{
			Start();
		}

		inline static std::unique_ptr<Timer> m_pInstance{};

		const double m_MaxElapsedSeconds;
		double m_ElapsedSeconds;
		double m_TotalElapsedSeconds;
		int m_FPS;
		int m_FPSCounter;
		double m_FPSTimer;
		double m_TimePerFrame;

		Timepoint m_StartTimepoint;
		Timepoint m_PreviousTimepoint;
	};
}