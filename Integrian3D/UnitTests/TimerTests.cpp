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

	Timepoint t1{}, t2{};

	REQUIRE(t1 == t2);
	REQUIRE((t1 + t2).Count() == 0);
	REQUIRE((t2 + t1).Count() == 0);
	REQUIRE((t1 - t2).Count() == 0);
	REQUIRE((t2 - t1).Count() == 0);

	t1 = Timepoint{ 5_ms };

	REQUIRE(t1 != t2);
	REQUIRE((t1 + t2).Count() == 5);
	REQUIRE((t2 + t1).Count() == 5);
	REQUIRE((t1 - t2).Count() == 5);
	REQUIRE((t2 - t1).Count() == -5);

	t2 += t1;

	REQUIRE(t1.Count() == 5);
	REQUIRE(t2.Count() == 5);
	REQUIRE(t1 == t2);

	t1 -= t2;

	REQUIRE(t1.Count() == 0.f);
	REQUIRE(t2.Count() == 5.f);
	REQUIRE(t1 != t2);
}

TEST_CASE("Testing Timer")
{
	using namespace Integrian3D::Time;
	using namespace Integrian3D::Math;

	Timer& timer{ Timer::GetInstance() };
	REQUIRE(AreEqual(timer.GetElapsedSeconds(), 0.f));
	REQUIRE(timer.Now().Count() != 0);

	timer.Start();
	REQUIRE(AreEqual(timer.GetElapsedSeconds(), 0.f));
	REQUIRE(timer.Now().Count() != 0);

	Timepoint t1{ timer.Now() };

	Sleep(1000);

	timer.Update();
	REQUIRE(timer.Now() > t1);

	Timepoint t2{ timer.Now() };
	REQUIRE(t2 > t1);
	REQUIRE(t1 < t2);
}