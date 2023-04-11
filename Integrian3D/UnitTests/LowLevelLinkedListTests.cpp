#include <catch.hpp>
#include "../Memory/LowLevelLL/LowLevelLL.h"

struct LowLevelLL_Tests
{
	LowLevelLL_Tests() = default;
	LowLevelLL_Tests(int a, double b, std::string c)
		: m_A{ a }
		, m_B{ b }
		, m_C{ c }
	{}

	int m_A{ -512 };
	double m_B{ 0.00562 };
	std::string m_C{ "Hello World" };

	bool operator==(const LowLevelLL_Tests& other) const
	{
		return m_A == other.m_A;
	}
};

TEST_CASE("Testing the low level linked list")
{
	using namespace Integrian3D::Memory;

	SECTION("Using integers")
	{
		LowLevelLL<int> list{};

		REQUIRE(list.Count() == 0);
		REQUIRE(list.Contains(6) == false);

		list.Add(6);
		REQUIRE(list.Count() == 1);
		REQUIRE(list.Get(0) == 6);
		REQUIRE(list.Back() == 6);
		REQUIRE(list.Contains(6) == true);

		list.Add(2);
		REQUIRE(list.Count() == 2);
		REQUIRE(list.Get(0) == 6);
		REQUIRE(list.Get(1) == 2);
		REQUIRE(list.Back() == 2);
		REQUIRE(list.Contains(2) == true);

		list.Add(3);
		REQUIRE(list.Count() == 3);
		REQUIRE(list.Get(0) == 6);
		REQUIRE(list.Get(1) == 2);
		REQUIRE(list.Get(2) == 3);
		REQUIRE(list.Back() == 3);
		REQUIRE(list.Contains(3) == true);

		list.Remove(2);
		REQUIRE(list.Count() == 2);
		REQUIRE(list.Get(0) == 6);
		REQUIRE(list.Get(1) == 3);
		REQUIRE(list.Back() == 3);
		REQUIRE(list.Contains(2) == false);

		list.Clear();
		REQUIRE(list.Count() == 0);
		REQUIRE(list.Contains(6) == false);
	}

	SECTION("Using custom struct with heap allocated data")
	{
		LowLevelLL<LowLevelLL_Tests> list{};

		REQUIRE(list.Count() == 0);

		list.Add(LowLevelLL_Tests{});
		REQUIRE(list.Count() == 1);
		REQUIRE(list.Get(0).m_A == -512);
		REQUIRE(list.Get(0).m_B == 0.00562);
		REQUIRE(list.Get(0).m_C == "Hello World");

		list.Add(LowLevelLL_Tests{ 15,36.0,"Hello There" });
		REQUIRE(list.Count() == 2);
		REQUIRE(list.Get(0).m_A == -512);
		REQUIRE(list.Get(0).m_B == 0.00562);
		REQUIRE(list.Get(0).m_C == "Hello World");
		REQUIRE(list.Get(1).m_A == 15);
		REQUIRE(list.Get(1).m_B == 36.0);
		REQUIRE(list.Get(1).m_C == "Hello There");
		REQUIRE(list.Back().m_A == 15);
		REQUIRE(list.Back().m_B == 36.0);
		REQUIRE(list.Back().m_C == "Hello There");

		list.Add(LowLevelLL_Tests{ 27,85.0,"Excadrill" });
		REQUIRE(list.Count() == 3);
		REQUIRE(list.Get(0).m_A == -512);
		REQUIRE(list.Get(0).m_B == 0.00562);
		REQUIRE(list.Get(0).m_C == "Hello World");
		REQUIRE(list.Get(1).m_A == 15);
		REQUIRE(list.Get(1).m_B == 36.0);
		REQUIRE(list.Get(1).m_C == "Hello There");
		REQUIRE(list.Get(2).m_A == 27);
		REQUIRE(list.Get(2).m_B == 85.0);
		REQUIRE(list.Get(2).m_C == "Excadrill");

		list.Remove(LowLevelLL_Tests{ 15,36.0,"Hello There" });
		REQUIRE(list.Count() == 2);
		REQUIRE(list.Get(0).m_A == -512);
		REQUIRE(list.Get(0).m_B == 0.00562);
		REQUIRE(list.Get(0).m_C == "Hello World");
		REQUIRE(list.Get(1).m_A == 27);
		REQUIRE(list.Get(1).m_B == 85.0);
		REQUIRE(list.Get(1).m_C == "Excadrill");
		REQUIRE(list.Back().m_A == 27);
		REQUIRE(list.Back().m_B == 85.0);
		REQUIRE(list.Back().m_C == "Excadrill");

		list.Clear();
		REQUIRE(list.Count() == 0);
	}

	SECTION("Using custom struct with heap allocated data")
	{
		LowLevelLL<LowLevelLL_Tests> list{};

		REQUIRE(list.Count() == 0);

		list.Add(LowLevelLL_Tests{});
		REQUIRE(list.Count() == 1);
		REQUIRE(list.Get(0).m_A == -512);
		REQUIRE(list.Get(0).m_B == 0.00562);
		REQUIRE(list.Get(0).m_C == "Hello World");

		list.Add(LowLevelLL_Tests{ 15,36.0,"Hello There" });
		REQUIRE(list.Count() == 2);
		REQUIRE(list.Get(0).m_A == -512);
		REQUIRE(list.Get(0).m_B == 0.00562);
		REQUIRE(list.Get(0).m_C == "Hello World");
		REQUIRE(list.Get(1).m_A == 15);
		REQUIRE(list.Get(1).m_B == 36.0);
		REQUIRE(list.Get(1).m_C == "Hello There");
		REQUIRE(list.Back().m_A == 15);
		REQUIRE(list.Back().m_B == 36.0);
		REQUIRE(list.Back().m_C == "Hello There");

		list.Add(LowLevelLL_Tests{ 27,85.0,"Excadrill" });
		REQUIRE(list.Count() == 3);
		REQUIRE(list.Get(0).m_A == -512);
		REQUIRE(list.Get(0).m_B == 0.00562);
		REQUIRE(list.Get(0).m_C == "Hello World");
		REQUIRE(list.Get(1).m_A == 15);
		REQUIRE(list.Get(1).m_B == 36.0);
		REQUIRE(list.Get(1).m_C == "Hello There");
		REQUIRE(list.Get(2).m_A == 27);
		REQUIRE(list.Get(2).m_B == 85.0);
		REQUIRE(list.Get(2).m_C == "Excadrill");

		list.Remove(LowLevelLL_Tests{ 15,36.0,"Hello There" });
		REQUIRE(list.Count() == 2);
		REQUIRE(list.Get(0).m_A == -512);
		REQUIRE(list.Get(0).m_B == 0.00562);
		REQUIRE(list.Get(0).m_C == "Hello World");
		REQUIRE(list.Get(1).m_A == 27);
		REQUIRE(list.Get(1).m_B == 85.0);
		REQUIRE(list.Get(1).m_C == "Excadrill");
		REQUIRE(list.Back().m_A == 27);
		REQUIRE(list.Back().m_B == 85.0);
		REQUIRE(list.Back().m_C == "Excadrill");
	}

}