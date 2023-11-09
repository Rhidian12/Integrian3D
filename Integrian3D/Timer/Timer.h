#pragma once

#include "../EngineConstants.h"
#include "TimeLength.h"
#include "Timepoint/Timepoint.h"
#include "../Math/Math.h"

#include <memory> /* std::unique_ptr */

namespace Integrian3D::Time
{
	class Timer final
	{
	public:
		~Timer() = default;

		static Timer& GetInstance();

		Timer(const Timer&) noexcept = delete;
		Timer(Timer&&) noexcept = delete;
		Timer& operator=(const Timer&) noexcept = delete;
		Timer& operator=(Timer&&) noexcept = delete;

		void Start();
		void Update();

		__NODISCARD static Timepoint Now();

		// [CRINGE]: Very basic way of calculating FPS
		__NODISCARD int GetFPS() const;

		__NODISCARD float GetElapsedSeconds() const;
		__NODISCARD float GetTotalElapsedSeconds() const;
		__NODISCARD float GetFixedElapsedSeconds() const;

		__NODISCARD int64 GetElapsedTime() const;
		__NODISCARD int64 GetTotalElapsedTime() const;

	private:
		Timer();

		inline static std::unique_ptr<Timer> m_pInstance{};

		const int64 MaxElapsedTime;
		int64 ElapsedTime;
		int64 TotalElapsedTime;
		int m_FPS;
		int m_FPSCounter;
		float m_FPSTimer;
		float TimePerFrame;

		Timepoint m_StartTimepoint;
		Timepoint m_PreviousTimepoint;
	};
}