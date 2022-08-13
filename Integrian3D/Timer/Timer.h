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
		__NODISCARD float GetElapsedSeconds() const { return ElapsedSeconds; }
		__NODISCARD float GetFixedElapsedSeconds() const { return TimePerFrame; }
		__NODISCARD float GetTotalElapsedSeconds() const { return TotalElapsedSeconds; }
		__NODISCARD int GetFPS() const { return FPS; }
		__NODISCARD float GetTimePerFrame() const { return TimePerFrame; }

		/* Base templated function, should never be called */
		template<TimeLength T>
		float GetElapsedTime() const { return 0.f; };
		template<>
		__NODISCARD float GetElapsedTime<TimeLength::NanoSeconds>() const { return ElapsedSeconds * 216'000; }
		template<>
		__NODISCARD float GetElapsedTime<TimeLength::MicroSeconds>() const { return ElapsedSeconds * 3600; }
		template<>
		__NODISCARD float GetElapsedTime<TimeLength::MilliSeconds>() const { return ElapsedSeconds * 60; }
		template<>
		__NODISCARD float GetElapsedTime<TimeLength::Seconds>() const { return ElapsedSeconds; }
		template<>
		__NODISCARD float GetElapsedTime<TimeLength::Minutes>() const { return ElapsedSeconds / 60; }
		template<>
		__NODISCARD float GetElapsedTime<TimeLength::Hours>() const { return ElapsedSeconds / 3600; }

		/* Base templated function, should never be called */
		template<TimeLength T>
		__NODISCARD float GetFixedElapsedTime() const { return 0.f; }
		template<>
		__NODISCARD float GetFixedElapsedTime<TimeLength::NanoSeconds>() const { return TimePerFrame * 216'000; }
		template<>
		__NODISCARD float GetFixedElapsedTime<TimeLength::MicroSeconds>() const { return TimePerFrame * 3600; }
		template<>
		__NODISCARD float GetFixedElapsedTime<TimeLength::MilliSeconds>() const { return TimePerFrame * 60; }
		template<>
		__NODISCARD float GetFixedElapsedTime<TimeLength::Seconds>() const { return TimePerFrame; }
		template<>
		__NODISCARD float GetFixedElapsedTime<TimeLength::Minutes>() const { return TimePerFrame / 60; }
		template<>
		__NODISCARD float GetFixedElapsedTime<TimeLength::Hours>() const { return TimePerFrame / 3600; }

		/* Base templated function, should never be called */
		template<TimeLength T>
		__NODISCARD float GetTotalElapsedSeconds() const { return 0.f; }
		template<>
		__NODISCARD float GetTotalElapsedSeconds<TimeLength::NanoSeconds>() const { return TotalElapsedSeconds * 216'000; }
		template<>
		__NODISCARD float GetTotalElapsedSeconds<TimeLength::MicroSeconds>() const { return TotalElapsedSeconds * 3600; }
		template<>
		__NODISCARD float GetTotalElapsedSeconds<TimeLength::MilliSeconds>() const { return TotalElapsedSeconds * 60; }
		template<>
		__NODISCARD float GetTotalElapsedSeconds<TimeLength::Seconds>() const { return TotalElapsedSeconds; }
		template<>
		__NODISCARD float GetTotalElapsedSeconds<TimeLength::Minutes>() const { return TotalElapsedSeconds / 60; }
		template<>
		__NODISCARD float GetTotalElapsedSeconds<TimeLength::Hours>() const { return TotalElapsedSeconds / 3600; }

	private:
		Timer();

		friend std::unique_ptr<Timer> std::make_unique();
		inline static std::unique_ptr<Timer> Instance{};

		const float MaxElapsedSeconds;
		float ElapsedSeconds;
		float TotalElapsedSeconds;
		int FPS;
		int FPSCounter;
		float FPSTimer;
		float TimePerFrame;

		std::chrono::steady_clock::time_point StartTimepoint;
		std::chrono::steady_clock::time_point PreviousTimepoint;
	};
}