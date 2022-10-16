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
		__NODISCARD double GetElapsedSeconds() const { return ElapsedSeconds; }
		__NODISCARD double GetFixedElapsedSeconds() const { return TimePerFrame; }
		__NODISCARD double GetTotalElapsedSeconds() const { return TotalElapsedSeconds; }
		__NODISCARD int GetFPS() const { return FPS; }
		__NODISCARD double GetTimePerFrame() const { return TimePerFrame; }

		/* Base templated function, should never be called */
		template<TimeLength T>
		__NODISCARD double GetElapsedTime() const { return 0; };
		template<>
		__NODISCARD double GetElapsedTime<TimeLength::NanoSeconds>() const { return ElapsedSeconds * 216'000; }
		template<>
		__NODISCARD double GetElapsedTime<TimeLength::MicroSeconds>() const { return ElapsedSeconds * 3600; }
		template<>
		__NODISCARD double GetElapsedTime<TimeLength::MilliSeconds>() const { return ElapsedSeconds * 60; }
		template<>
		__NODISCARD double GetElapsedTime<TimeLength::Seconds>() const { return ElapsedSeconds; }
		template<>
		__NODISCARD double GetElapsedTime<TimeLength::Minutes>() const { return ElapsedSeconds / 60; }
		template<>
		__NODISCARD double GetElapsedTime<TimeLength::Hours>() const { return ElapsedSeconds / 3600; }

		/* Base templated function, should never be called */
		template<TimeLength T>
		__NODISCARD double GetFixedElapsedTime() const { return 0; }
		template<>
		__NODISCARD double GetFixedElapsedTime<TimeLength::NanoSeconds>() const { return TimePerFrame * 216'000; }
		template<>
		__NODISCARD double GetFixedElapsedTime<TimeLength::MicroSeconds>() const { return TimePerFrame * 3600; }
		template<>
		__NODISCARD double GetFixedElapsedTime<TimeLength::MilliSeconds>() const { return TimePerFrame * 60; }
		template<>
		__NODISCARD double GetFixedElapsedTime<TimeLength::Seconds>() const { return TimePerFrame; }
		template<>
		__NODISCARD double GetFixedElapsedTime<TimeLength::Minutes>() const { return TimePerFrame / 60; }
		template<>
		__NODISCARD double GetFixedElapsedTime<TimeLength::Hours>() const { return TimePerFrame / 3600; }

		/* Base templated function, should never be called */
		template<TimeLength T>
		__NODISCARD double GetTotalElapsedSeconds() const { return 0; }
		template<>
		__NODISCARD double GetTotalElapsedSeconds<TimeLength::NanoSeconds>() const { return TotalElapsedSeconds * 216'000; }
		template<>
		__NODISCARD double GetTotalElapsedSeconds<TimeLength::MicroSeconds>() const { return TotalElapsedSeconds * 3600; }
		template<>
		__NODISCARD double GetTotalElapsedSeconds<TimeLength::MilliSeconds>() const { return TotalElapsedSeconds * 60; }
		template<>
		__NODISCARD double GetTotalElapsedSeconds<TimeLength::Seconds>() const { return TotalElapsedSeconds; }
		template<>
		__NODISCARD double GetTotalElapsedSeconds<TimeLength::Minutes>() const { return TotalElapsedSeconds / 60; }
		template<>
		__NODISCARD double GetTotalElapsedSeconds<TimeLength::Hours>() const { return TotalElapsedSeconds / 3600; }

	private:
		Timer();

		friend std::unique_ptr<Timer> std::make_unique();
		inline static std::unique_ptr<Timer> Instance{};

		const double MaxElapsedSeconds;
		double ElapsedSeconds;
		double TotalElapsedSeconds;
		int FPS;
		int FPSCounter;
		double FPSTimer;
		double TimePerFrame;

		std::chrono::steady_clock::time_point StartTimepoint;
		std::chrono::steady_clock::time_point PreviousTimepoint;
	};
}