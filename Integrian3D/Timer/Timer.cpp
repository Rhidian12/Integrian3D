#include "Timer.h"

#pragma warning ( push )
#pragma warning ( disable : 4005 ) /* warning C4005: 'APIENTRY': macro redefinition */ 
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
#pragma warning ( pop )
#ifdef min
#undef min
#endif

namespace Integrian3D::Time
{
	Timer::Timer()
		: MaxElapsedTime{ 100 }
		, ElapsedTime{}
		, TotalElapsedTime{}
		, m_FPS{}
		, m_FPSCounter{}
		, m_FPSTimer{}
		, TimePerFrame{ 1.f / 60.f }
		, m_StartTimepoint{}
		, m_PreviousTimepoint{}
	{
		Start();
	}

	Timer& Timer::GetInstance()
	{
		if (!m_pInstance)
			m_pInstance.reset(new Timer{});

		return *m_pInstance.get();
	}


	void Timer::Start()
	{
		m_PreviousTimepoint = Now();
	}

	void Timer::Update()
	{
		m_StartTimepoint = Now();

		ElapsedTime = (m_StartTimepoint - m_PreviousTimepoint).Count();
		ElapsedTime = std::min(ElapsedTime, MaxElapsedTime);

		TotalElapsedTime += ElapsedTime;

		m_PreviousTimepoint = m_StartTimepoint;

		LOG(Log, LogErrorLevel::Log, "Elapsed Seconds {}", ElapsedTime);
		m_FPS = static_cast<int>(1.f / ElapsedTime);
	}

	Timepoint Timer::Now()
	{
		const int64 frequency{ _Query_perf_frequency() };
		const int64 counter{ _Query_perf_counter() };

		static constexpr int64 NanoToMilliRatio = 1'000'000;
		static constexpr int64 NanoRatio = 1'000'000'000;

		// Instead of just having "(counter * static_cast<int64_t>(SecToNano)) / frequency",
		// the algorithm below prevents overflow when counter is sufficiently large.
		// It assumes that frequency * static_cast<int64_t>(SecToNano) does not overflow, which is currently true for nano period.
		// It is not realistic for counter to accumulate to large values from zero with this assumption,
		// but the initial value of counter could be large.

		const int64_t whole = (counter / frequency) * NanoRatio;
		const int64_t part = (counter % frequency) * NanoRatio / frequency;
		return Timepoint{ Detail::_Milliseconds{ (whole + part) / NanoToMilliRatio } };
	}

	int Timer::GetFPS() const
	{
		return m_FPS;
	}

	float Timer::GetElapsedSeconds() const
	{
		return ConvertMilliToSeconds(ElapsedTime);
	}

	float Timer::GetTotalElapsedSeconds() const
	{
		return ConvertMilliToSeconds(TotalElapsedTime);
	}

	float Timer::GetFixedElapsedSeconds() const
	{
		return TimePerFrame;
	}

	int64 Timer::GetElapsedTime() const
	{
		return ElapsedTime;
	}

	int64 Timer::GetTotalElapsedTime() const
	{
		return TotalElapsedTime;
	}
}