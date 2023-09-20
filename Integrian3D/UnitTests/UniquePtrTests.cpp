#include <catch.hpp>

#include "Memory/UniquePtr.h"

using namespace Integrian3D;

TEST_CASE("Creating Unique Pointer For Basic Types")
{
	SECTION("Creating Empty int Pointer")
	{
		const UniquePtr<int> Pointer{};

		REQUIRE(Pointer.Get() == nullptr);
		REQUIRE(static_cast<bool>(Pointer) == false);
	}

	SECTION("Creating int pointer with MakeUnique")
	{
		const UniquePtr<int> Pointer{ MakeUnique<int>(5) };

		REQUIRE(Pointer.Get() != nullptr);
		REQUIRE(static_cast<bool>(Pointer) == true);
		REQUIRE(*Pointer == 5);
		REQUIRE(*Pointer.Get() == 5);
	}

	SECTION("Creating int pointer and then changing it")
	{
		int* IntPointer{ new int{ 5 } };
		UniquePtr<int> Pointer{ __MOVE(IntPointer) };

		REQUIRE(Pointer.Get() != nullptr);
		REQUIRE(static_cast<bool>(Pointer) == true);
		REQUIRE(*Pointer == 5);
		REQUIRE(*Pointer.Get() == 5);

		*Pointer = 10;

		REQUIRE(Pointer.Get() != nullptr);
		REQUIRE(static_cast<bool>(Pointer) == true);
		REQUIRE(*Pointer == 10);
		REQUIRE(*Pointer.Get() == 10);
	}

	SECTION("Creating int pointer and then assigning another pointer with Reset")
	{
		UniquePtr<int> Pointer{ MakeUnique<int>(5) };

		Pointer.Reset(new int{ 10 });

		REQUIRE(Pointer.Get() != nullptr);
		REQUIRE(static_cast<bool>(Pointer) == true);
		REQUIRE(*Pointer == 10);
		REQUIRE(*Pointer.Get() == 10);
	}

	SECTION("Creating int pointer and then moving it to another unique pointer")
	{
		{
			UniquePtr<int> Pointer{ MakeUnique<int>(5) };

			UniquePtr<int> MoveCtorPointer{ __MOVE(Pointer) };

			REQUIRE(Pointer.Get() == nullptr);
			REQUIRE(static_cast<bool>(Pointer) == false);

			REQUIRE(MoveCtorPointer.Get() != nullptr);
			REQUIRE(static_cast<bool>(MoveCtorPointer) == true);
			REQUIRE(*MoveCtorPointer == 5);
			REQUIRE(*MoveCtorPointer.Get() == 5);
		}

		{
			UniquePtr<int> Pointer{ MakeUnique<int>(5) };

			UniquePtr<int> MoveOpPointer = __MOVE(Pointer);

			REQUIRE(Pointer.Get() == nullptr);
			REQUIRE(static_cast<bool>(Pointer) == false);

			REQUIRE(MoveOpPointer.Get() != nullptr);
			REQUIRE(static_cast<bool>(MoveOpPointer) == true);
			REQUIRE(*MoveOpPointer == 5);
			REQUIRE(*MoveOpPointer.Get() == 5);
		}
	}
}

class TestData final
{
public:
	TestData() : Data{} {}
	TestData(int Value) : Data{ new int{ Value } } {}

	TestData(const TestData& Other) noexcept
		: Data{ new int{ *Other.Data } }
	{
	}
	TestData(TestData&& Other) noexcept
		: Data{ __MOVE(Other.Data) }
	{
		Other.Data = nullptr;
	}
	TestData& operator=(const TestData& Other) noexcept
	{
		if (Data)
		{
			delete Data;
			Data = nullptr;
		}

		Data = new int{ *Other.Data };

		return *this;
	}
	TestData& operator=(TestData&& Other) noexcept
	{
		if (Data)
		{
			delete Data;
			Data = nullptr;
		}

		Data = __MOVE(Other.Data);
		Other.Data = nullptr;

		return *this;
	}

	~TestData()
	{
		if (Data)
		{
			delete Data;
			Data = nullptr;
		}
	}

	int* GetData() { return Data; }
	const int* GetData() const { return Data; }

private:
	int* Data;
};

TEST_CASE("Creating Unique Pointer For Custom Types")
{
	SECTION("Creating Empty Custom Pointer")
	{
		const UniquePtr<TestData> Pointer{};

		REQUIRE(Pointer.Get() == nullptr);
		REQUIRE(static_cast<bool>(Pointer) == false);
	}

	SECTION("Creating Custom pointer with MakeUnique")
	{
		const UniquePtr<TestData> Pointer{ MakeUnique<TestData>(5) };

		REQUIRE(Pointer.Get() != nullptr);
		REQUIRE(static_cast<bool>(Pointer) == true);
		REQUIRE(*Pointer->GetData() == 5);
		REQUIRE(*Pointer.Get()->GetData() == 5);
	}

	SECTION("Creating Custom pointer and then changing it")
	{
		TestData* IntPointer{ new TestData{ 5 } };
		UniquePtr<TestData> Pointer{ __MOVE(IntPointer) };

		REQUIRE(Pointer.Get() != nullptr);
		REQUIRE(static_cast<bool>(Pointer) == true);
		REQUIRE(*Pointer->GetData() == 5);
		REQUIRE(*Pointer.Get()->GetData() == 5);

		*Pointer = TestData{ 10 };

		REQUIRE(Pointer.Get() != nullptr);
		REQUIRE(static_cast<bool>(Pointer) == true);
		REQUIRE(*Pointer->GetData() == 10);
		REQUIRE(*Pointer.Get()->GetData() == 10);
	}

	SECTION("Creating Custom pointer and then assigning another pointer with Reset")
	{
		UniquePtr<TestData> Pointer{ MakeUnique<TestData>(5) };

		Pointer.Reset(new TestData{ 10 });

		REQUIRE(Pointer.Get() != nullptr);
		REQUIRE(static_cast<bool>(Pointer) == true);
		REQUIRE(*Pointer->GetData() == 10);
		REQUIRE(*Pointer.Get()->GetData() == 10);
	}

	SECTION("Creating Custom pointer and then moving it to another unique pointer")
	{
		{
			UniquePtr<TestData> Pointer{ MakeUnique<TestData>(5) };

			UniquePtr<TestData> MoveCtorPointer{ __MOVE(Pointer) };

			REQUIRE(Pointer.Get() == nullptr);
			REQUIRE(static_cast<bool>(Pointer) == false);

			REQUIRE(MoveCtorPointer.Get() != nullptr);
			REQUIRE(static_cast<bool>(MoveCtorPointer) == true);
			REQUIRE(*MoveCtorPointer->GetData() == 5);
			REQUIRE(*MoveCtorPointer.Get()->GetData() == 5);
		}

		{
			UniquePtr<TestData> Pointer{ MakeUnique<TestData>(5) };

			UniquePtr<TestData> MoveOpPointer = __MOVE(Pointer);

			REQUIRE(Pointer.Get() == nullptr);
			REQUIRE(static_cast<bool>(Pointer) == false);

			REQUIRE(MoveOpPointer.Get() != nullptr);
			REQUIRE(static_cast<bool>(MoveOpPointer) == true);
			REQUIRE(*MoveOpPointer->GetData() == 5);
			REQUIRE(*MoveOpPointer.Get()->GetData() == 5);
		}
	}
}

namespace Integrian3D::Win32Utils
{
	class Win32Handle;
}

TEST_CASE("Creating Unique Pointer For Forward Declared Types")
{
	SECTION("Creating Empty Custom Pointer")
	{
		const UniquePtr<Integrian3D::Win32Utils::Win32Handle> Pointer{};

		REQUIRE(Pointer.Get() == nullptr);
		REQUIRE(static_cast<bool>(Pointer) == false);
	}
}

TEST_CASE("Creating Unique Pointer For Int Array Types")
{
	SECTION("Creating Empty Int Array Pointer")
	{
		const UniquePtr<int[]> Pointer{};

		REQUIRE(Pointer.Get() == nullptr);
		REQUIRE(static_cast<bool>(Pointer) == false);
	}

	SECTION("Creating Int Array pointer with MakeUnique")
	{
		const UniquePtr<int[]> Pointer{ MakeUnique<int[]>(5) };

		REQUIRE(Pointer.Get() != nullptr);
		REQUIRE(static_cast<bool>(Pointer) == true);
	}

	SECTION("Creating int pointer and then assigning another pointer with Reset")
	{
		UniquePtr<int[]> Pointer{ MakeUnique<int[]>(5) };

		Pointer.Reset(new int[10]{});

		REQUIRE(Pointer.Get() != nullptr);
		REQUIRE(static_cast<bool>(Pointer) == true);
	}

	SECTION("Creating int pointer and then moving it to another unique pointer")
	{
		{
			UniquePtr<int[]> Pointer{ MakeUnique<int[]>(5) };

			UniquePtr<int[]> MoveCtorPointer{ __MOVE(Pointer) };

			REQUIRE(Pointer.Get() == nullptr);
			REQUIRE(static_cast<bool>(Pointer) == false);

			REQUIRE(MoveCtorPointer.Get() != nullptr);
			REQUIRE(static_cast<bool>(MoveCtorPointer) == true);
		}

		{
			UniquePtr<int[]> Pointer{ MakeUnique<int[]>(5) };

			UniquePtr<int[]> MoveOpPointer = __MOVE(Pointer);

			REQUIRE(Pointer.Get() == nullptr);
			REQUIRE(static_cast<bool>(Pointer) == false);

			REQUIRE(MoveOpPointer.Get() != nullptr);
			REQUIRE(static_cast<bool>(MoveOpPointer) == true);
		}
	}
}