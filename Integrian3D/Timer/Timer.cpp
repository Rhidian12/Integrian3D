#include "Timer.h"

namespace Integrian3D
{
	Timer::Timer()
		: MaxElapsedSeconds{ 0.1f }
		, ElapsedSeconds{}
		, TotalElapsedSeconds{}
		, FPS{}
		, FPSCounter{}
		, FPSTimer{}
		, TimePerFrame{ 1.f / 60.f }
	{
		Start();
	}

	Timer& Timer::GetInstance()
	{
		if (!Instance)
		{
			Instance = std::make_unique<Timer>();
		}

		return *Instance.get();
	}

	void Timer::Start()
	{
		PreviousTimepoint = std::chrono::steady_clock::now();
	}

	void Timer::Update()
	{
		StartTimepoint = std::chrono::steady_clock::now();
		
		ElapsedSeconds = std::chrono::duration<float>(StartTimepoint - PreviousTimepoint).count();
		ElapsedSeconds = std::min(ElapsedSeconds, MaxElapsedSeconds);
		
		TotalElapsedSeconds += ElapsedSeconds;

		PreviousTimepoint = StartTimepoint;

		FPS = static_cast<int>(1.f / ElapsedSeconds);
	}
}