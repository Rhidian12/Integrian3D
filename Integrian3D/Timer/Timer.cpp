#include "Timer.h"

namespace Integrian3D
{
	Timer::Timer()
		: m_MaxElapsedSeconds{ 0.1 }
		, m_ElapsedSeconds{}
		, m_TotalElapsedSeconds{}
		, m_FPS{}
		, m_FPSCounter{}
		, m_FPSTimer{}
		, m_TimePerFrame{ 1.0 / 60.0 }
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
		m_PreviousTimepoint = std::chrono::steady_clock::now();
	}

	void Timer::Update()
	{
		m_StartTimepoint = std::chrono::steady_clock::now();
		
		m_ElapsedSeconds = std::chrono::duration<double>(m_StartTimepoint - m_PreviousTimepoint).count();
		m_ElapsedSeconds = std::min(m_ElapsedSeconds, m_MaxElapsedSeconds);
		
		m_TotalElapsedSeconds += m_ElapsedSeconds;

		m_PreviousTimepoint = m_StartTimepoint;

		m_FPS = static_cast<int>(1.0 / m_ElapsedSeconds);
	}
}