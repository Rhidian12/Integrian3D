#pragma once

#include "../EngineConstants.h"

#include <chrono> /* std::chrono::steady_clock */
#include <memory> /* std::unique_ptr */

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

	class Timer final
	{
	public:
		static Timer& GetInstance();

		Timer(const Timer&) noexcept = delete;
		Timer(Timer&&) noexcept = delete;
		Timer& operator=(const Timer&) noexcept = delete;
		Timer& operator=(Timer&&) noexcept = delete;

		void Start();

		void Update();

		__NODISCARD double GetElapsedSeconds() const { return m_ElapsedSeconds; }
		__NODISCARD double GetFixedElapsedSeconds() const { return m_TimePerFrame; }
		__NODISCARD double GetTotalElapsedSeconds() const { return m_TotalElapsedSeconds; }
		__NODISCARD int GetFPS() const { return m_FPS; }
		__NODISCARD double GetTimePerFrame() const { return m_TimePerFrame; }

#pragma region GetElapsedTime
		/* Base templated function, should never be called */
		template<TimeLength T>
		__NODISCARD double GetElapsedTime() const { return 0; };
		template<>
		__NODISCARD double GetElapsedTime<TimeLength::NanoSeconds>() const { return m_ElapsedSeconds * 216'000; }
		template<>
		__NODISCARD double GetElapsedTime<TimeLength::MicroSeconds>() const { return m_ElapsedSeconds * 3600; }
		template<>
		__NODISCARD double GetElapsedTime<TimeLength::MilliSeconds>() const { return m_ElapsedSeconds * 60; }
		template<>
		__NODISCARD double GetElapsedTime<TimeLength::Seconds>() const { return m_ElapsedSeconds; }
		template<>
		__NODISCARD double GetElapsedTime<TimeLength::Minutes>() const { return m_ElapsedSeconds / 60; }
		template<>
		__NODISCARD double GetElapsedTime<TimeLength::Hours>() const { return m_ElapsedSeconds / 3600; }
#pragma endregion

#pragma region GetFixedElapsedTime
		/* Base templated function, should never be called */
		template<TimeLength T>
		__NODISCARD double GetFixedElapsedTime() const { return 0; }
		template<>
		__NODISCARD double GetFixedElapsedTime<TimeLength::NanoSeconds>() const { return m_TimePerFrame * 216'000; }
		template<>
		__NODISCARD double GetFixedElapsedTime<TimeLength::MicroSeconds>() const { return m_TimePerFrame * 3600; }
		template<>
		__NODISCARD double GetFixedElapsedTime<TimeLength::MilliSeconds>() const { return m_TimePerFrame * 60; }
		template<>
		__NODISCARD double GetFixedElapsedTime<TimeLength::Seconds>() const { return m_TimePerFrame; }
		template<>
		__NODISCARD double GetFixedElapsedTime<TimeLength::Minutes>() const { return m_TimePerFrame / 60; }
		template<>
		__NODISCARD double GetFixedElapsedTime<TimeLength::Hours>() const { return m_TimePerFrame / 3600; }
#pragma endregion

#pragma region GetTotalElapsedTime
		/* Base templated function, should never be called */
		template<TimeLength T>
		__NODISCARD double GetTotalElapsedTime() const { return 0; }
		template<>
		__NODISCARD double GetTotalElapsedTime<TimeLength::NanoSeconds>() const { return m_TotalElapsedSeconds * 216'000; }
		template<>
		__NODISCARD double GetTotalElapsedTime<TimeLength::MicroSeconds>() const { return m_TotalElapsedSeconds * 3600; }
		template<>
		__NODISCARD double GetTotalElapsedTime<TimeLength::MilliSeconds>() const { return m_TotalElapsedSeconds * 60; }
		template<>
		__NODISCARD double GetTotalElapsedTime<TimeLength::Seconds>() const { return m_TotalElapsedSeconds; }
		template<>
		__NODISCARD double GetTotalElapsedTime<TimeLength::Minutes>() const { return m_TotalElapsedSeconds / 60; }
		template<>
		__NODISCARD double GetTotalElapsedTime<TimeLength::Hours>() const { return m_TotalElapsedSeconds / 3600; }
#pragma endregion

	private:
		Timer();

		inline static std::unique_ptr<Timer> m_pInstance{};

		const double m_MaxElapsedSeconds;
		double m_ElapsedSeconds;
		double m_TotalElapsedSeconds;
		int m_FPS;
		int m_FPSCounter;
		double m_FPSTimer;
		double m_TimePerFrame;

		std::chrono::steady_clock::time_point m_StartTimepoint;
		std::chrono::steady_clock::time_point m_PreviousTimepoint;
	};
}