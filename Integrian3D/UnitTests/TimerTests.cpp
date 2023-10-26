#include <catch.hpp>

#include "Timer/Timer.h"
#include "Timer/Timepoint/Timepoint.h"
#include "Math/Math.h"

#ifdef _WIN32
I_DISABLE_WARNING(4005) // warning C4005: 'APIENTRY': macro redefinition
#include <Windows.h>
I_ENABLE_WARNING(4005)
#endif
#include <type_traits>

TEST_CASE("Testing Basic Timepoints")
{
	using namespace Integrian3D::Math;
	using namespace Integrian3D::Time;

	constexpr float epsilon = 0.00001f;

	Timepoint t1{}, t2{};

	REQUIRE(t1 == t2);
	REQUIRE(AreEqual((t1 + t2).Count(), 0.f, epsilon));
	REQUIRE(AreEqual((t2 + t1).Count(), 0.f, epsilon));
	REQUIRE(AreEqual((t1 - t2).Count(), 0.f, epsilon));
	REQUIRE(AreEqual((t2 - t1).Count(), 0.f, epsilon));

	t1 = Timepoint{ 5.0 };

	REQUIRE(t1 != t2);
	REQUIRE(AreEqual((t1 + t2).Count(), 5.f, epsilon));
	REQUIRE(AreEqual((t2 + t1).Count(), 5.f, epsilon));
	REQUIRE(AreEqual((t1 - t2).Count(), 5.f, epsilon));
	REQUIRE(AreEqual((t2 - t1).Count(), -5.f, epsilon));

	REQUIRE(std::is_same_v<uint64_t, decltype((t1 + t2).Count<TimeLength::Seconds, uint64_t>())>);

	REQUIRE(AreEqual((t1 + t2).Count<TimeLength::NanoSeconds>(), 5'000'000'000.f, epsilon));
	REQUIRE(AreEqual((t1 + t2).Count<TimeLength::MicroSeconds>(), 5'000'000.f, epsilon));
	REQUIRE(AreEqual((t1 + t2).Count<TimeLength::MilliSeconds>(), 5'000.f, epsilon));
	REQUIRE(AreEqual((t1 + t2).Count<TimeLength::Minutes>(), 0.083333f, epsilon));
	REQUIRE(AreEqual((t1 + t2).Count<TimeLength::Hours>(), 0.001389f, epsilon));

	t2 += t1;

	REQUIRE(AreEqual(t1.Count(), 5.f, epsilon));
	REQUIRE(AreEqual(t2.Count(), 5.f, epsilon));
	REQUIRE(t1 == t2);

	t1 -= t2;

	REQUIRE(AreEqual(t1.Count(), 0.f, epsilon));
	REQUIRE(AreEqual(t2.Count(), 5.f, epsilon));
	REQUIRE(t1 != t2);
}

TEST_CASE("Testing Timer")
{
	using namespace Integrian3D::Time;
	using namespace Integrian3D::Math;

	Timer& timer{ Timer::GetInstance() };
	REQUIRE(AreEqual(timer.GetElapsedSeconds(), 0.f));
	REQUIRE(!AreEqual(timer.Now().Count(), 0.f));

	timer.Start();
	REQUIRE(AreEqual(timer.GetElapsedSeconds(), 0.f));
	REQUIRE(!AreEqual(timer.Now().Count(), 0.f));

	Timepoint t1{ timer.Now() };

	Sleep(1000);

	timer.Update();
	REQUIRE(timer.Now() > t1);

	Timepoint t2{ timer.Now() };
	REQUIRE(t2 > t1);
	REQUIRE(t1 < t2);
}