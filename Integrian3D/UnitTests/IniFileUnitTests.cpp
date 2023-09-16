#include <catch.hpp>

#include "IO/Ini/IniFile.h"

using namespace Integrian3D;

TEST_CASE("Reading in a .ini file")
{
	const IniFile IniFile{ "Resources/TestIni.ini" };

	{
		bool bBoolean{};
		REQUIRE(IniFile.GetBoolean("SomeData", "bBoolean", bBoolean));
		REQUIRE(bBoolean == true);
	}

	{
		int32 SomeInteger{};
		REQUIRE(IniFile.GetInteger("SomeData", "ThisIsSomeInteger", SomeInteger));
		REQUIRE(SomeInteger == 16);
	}

	{
		std::string DataPath{};
		REQUIRE(IniFile.GetString("ThisIsAnotherHeader", "DataPath", DataPath));
		REQUIRE(DataPath == "SomeWhitespace");
	}

	{
		float Value{};
		REQUIRE(IniFile.GetFloat("/Test/UnitTest", "AFloatingPoint", Value));
		REQUIRE(Value == 13.59f);
	}
}