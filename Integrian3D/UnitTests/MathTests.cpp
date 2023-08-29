#include <catch.hpp>

#include "../Math/Math.h"

TEST_CASE("Testing Math functions")
{
	using namespace Integrian3D::Math;

	SECTION("Testing AreEqual")
	{
		REQUIRE(AreEqual(0, 0));
		REQUIRE(AreEqual(-0, -0));
		REQUIRE(AreEqual(0, -0));
		REQUIRE(AreEqual(-0, 0));
		REQUIRE(!AreEqual(0, 620));
		REQUIRE(!AreEqual(620, 0));
		REQUIRE(!AreEqual(-5, 0));
		REQUIRE(!AreEqual(0, -5));

		REQUIRE(AreEqual(5.0, 5.0));
		REQUIRE(AreEqual(0.0, 0.0));
		REQUIRE(AreEqual(-0.0, -0.0));
		REQUIRE(AreEqual(-0.0, 0.0));
		REQUIRE(AreEqual(0.0, -0.0));
		REQUIRE(!AreEqual(30.0, -620.0));
		REQUIRE(!AreEqual(30.0, 29.0));
		REQUIRE(!AreEqual(30.0, 29.50));

		REQUIRE(!AreEqual(30.f, 29.99f));
		REQUIRE(AreEqual(30.f, 29.99f, 0.1f));
		REQUIRE(!AreEqual(30.f, 29.89f, 0.1f));
	}

	SECTION("Testing Radians <=> Degrees")
	{
		REQUIRE(ToRadians(0) == 0);
		REQUIRE(ToDegrees(0) == 0);

		REQUIRE(AreEqual(ToRadians(180.0), Pi));
		REQUIRE(AreEqual(ToDegrees(Pi), 180.0));

		REQUIRE(AreEqual(ToDegrees(1.0), 57.29577, 0.00001));
		REQUIRE(AreEqual(ToDegrees(0.75), 42.97183, 0.00001));
		REQUIRE(AreEqual(ToDegrees(0.35), 20.05352, 0.00001));

		REQUIRE(AreEqual(ToRadians(90.0), Pi_2, 0.00001));
		REQUIRE(AreEqual(ToRadians(45.0), Pi_4, 0.00001));
		REQUIRE(AreEqual(ToRadians(20.0), 0.34906, 0.00001));
	}

	SECTION("Testing Clamp")
	{
		int a{ 5 };
		Clamp(a, 0, 10);
		REQUIRE(a == 5);

		Clamp(a, -5, 15);
		REQUIRE(a == 5);

		Clamp(a, 7, 150);
		REQUIRE(a == 7);

		Clamp(a, 0, 3);
		REQUIRE(a == 3);
	}

	SECTION("Testing Random floats, integers and vectors")
	{
		REQUIRE(IsInRange(RandomF(0.f, 150.f), 0.f, 151.f));
		REQUIRE(IsInRange(RandomF(-50.f, 50.f), -50.f, 50.f));

		REQUIRE(IsInRange(RandomI(0, 1), 0, 2));
		REQUIRE(IsInRange(RandomI(0, 10), 0, 11));

		const Vec2D randomVec2{ RandomVec2D(-5.0, 10.0) };
		REQUIRE(IsInRange(randomVec2.x, -5.0, 10.0));
		REQUIRE(IsInRange(randomVec2.y, -5.0, 10.0));

		const Vec3D randomVec3{ RandomVec3D(-5.0, 10.0) };
		REQUIRE(IsInRange(randomVec3.x, -5.0, 10.0));
		REQUIRE(IsInRange(randomVec3.y, -5.0, 10.0));
		REQUIRE(IsInRange(randomVec3.z, -5.0, 10.0));

		const Vec4D randomVec4{ RandomVec4D(-5.0, 10.0) };
		REQUIRE(IsInRange(randomVec4.x, -5.0, 10.0));
		REQUIRE(IsInRange(randomVec4.y, -5.0, 10.0));
		REQUIRE(IsInRange(randomVec4.z, -5.0, 10.0));
		REQUIRE(IsInRange(randomVec4.w, -5.0, 10.0));
	}

	SECTION("Testing Max and Min")
	{
		REQUIRE(Max(0, 5) == 5);
		REQUIRE(Max(-9, 16) == 16);
		REQUIRE(Max(-9, -32) != -32);
		REQUIRE(Max(-9, -32) == -9);
		REQUIRE(Max(-19230, 1503) != -19230);
		REQUIRE(Max(-19230, 1503) == 1503);

		REQUIRE(Min(0, 5) == 0);
		REQUIRE(Min(512, 681) == 512);
		REQUIRE(Min(512, 681) != 681);
		REQUIRE(Min(-815, 0) == -815);
		REQUIRE(Min(-815, -816) == -816);
		REQUIRE(Min(-815, -816) != -815);
	}

	SECTION("Testing GDC")
	{
		REQUIRE(GetGCD<5, 15>() == 5);
		REQUIRE(GetGCD<12, 66>() == 6);
		REQUIRE(GetGCD<9, 99>() == 9);

		REQUIRE(GetGCD(5, 15) == 5);
		REQUIRE(GetGCD(12, 66) == 6);
		REQUIRE(GetGCD(9, 99) == 9);
	}
}
