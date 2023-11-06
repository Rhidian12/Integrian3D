#include <catch.hpp>

#include "Array/Array.h"
#include "DebugUtility.h"
#include "TPair/TPair.full.h"

#include <string>

TEST_CASE("Testing Pair")
{
	using namespace Integrian3D;

	{
		TPair<std::string, int> Pair{};

		REQUIRE(Pair.Key() == "");
		REQUIRE(Pair.Value() == 0);

		Pair.Key() = "Hello!";

		REQUIRE(Pair.Key() == "Hello!");

		Pair.Value() = 1655;

		REQUIRE(Pair.Value() == 1655);
	}

	{
		TPair<std::string, int> Pair{ MakePair<std::string, int>("Hello!", 1655) };

		REQUIRE(Pair.Key() == "Hello!");
		REQUIRE(Pair.Value() == 1655);
	}
}

TEST_CASE("Testing Pair with Custom Destructor")
{
	using namespace Integrian3D;

	struct PairTestStruct
	{
		PairTestStruct(int i)
		{
			Val = i;
		}

		PairTestStruct(const PairTestStruct& Other) noexcept
			: Val{ Other.Val }
		{}
		PairTestStruct(PairTestStruct&& Other) noexcept
			: Val{ std::move(Other.Val) }
		{}
		PairTestStruct& operator=(const PairTestStruct& Other) noexcept
		{
			Val = Other.Val;

			return *this;
		}
		PairTestStruct& operator=(PairTestStruct&& Other) noexcept
		{
			Val = std::move(Other.Val);

			return *this;
		}

		int Val;

		bool operator==(const PairTestStruct& Other) const
		{
			return Val == Other.Val;
		}
	};

	{
		PairTestStruct TestVal{ 66 };

		TPair<PairTestStruct, int> Pair{ MakePair(TestVal, 15) };

		REQUIRE(Pair.Key() == TestVal);
		REQUIRE(Pair.Value() == 15);
	}

	TArray<TPair<PairTestStruct, std::string>> ArrPairs{};
	constexpr static int32 NrOfElements = 10;

	for (int32 i{}; i < NrOfElements; ++i)
	{
		ArrPairs.Add(MakePair(PairTestStruct{ i }, std::to_string(i)));
	}

	for (int32 i{}; i < NrOfElements; ++i)
	{
		REQUIRE(ArrPairs[i].Key() == PairTestStruct{ i });
		REQUIRE(ArrPairs[i].Value() == std::to_string(i));
	}

	//ArrPairs.Erase([](const TPair<PairTestStruct, std::string>& Pair)->bool
	//	{
	//		return Pair.Key().Val == 3;
	//	});

	//REQUIRE(ArrPairs.Size() == NrOfElements - 1);

	//ArrPairs.Erase([](const TPair<PairTestStruct, std::string>& Pair)->bool
	//	{
	//		return Pair.Value() == "4";
	//	});

	//REQUIRE(ArrPairs.Size() == NrOfElements - 2);
}