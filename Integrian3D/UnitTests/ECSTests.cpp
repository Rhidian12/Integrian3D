#include <catch.hpp>

#include "ECS/Registry/Registry.h"
#include "Memory/UniquePtr.h"

struct BaseComponent
{
	virtual ~BaseComponent() = default;
};

struct ECSTestComponent : BaseComponent
{
	ECSTestComponent()
	{
		HeapAllocatedInt = Integrian3D::MakeUnique<int>();
	}
	ECSTestComponent(const ECSTestComponent&) noexcept = delete;
	ECSTestComponent(ECSTestComponent&& Other) noexcept
		: HeapAllocatedInt{ I_MOVE(Other.HeapAllocatedInt) }
	{}
	ECSTestComponent& operator=(const ECSTestComponent&) noexcept = delete;
	ECSTestComponent& operator=(ECSTestComponent&& Other) noexcept
	{
		HeapAllocatedInt = I_MOVE(Other.HeapAllocatedInt);

		return *this;
	}

	Integrian3D::UniquePtr<int> HeapAllocatedInt;
};

TEST_CASE("Testing ECS with some dummy component")
{
	using namespace Integrian3D;

	Registry Registry{};

	REQUIRE(Registry.GetAmountOfEntities() == 0);

	for (int32 i{}; i < 10; ++i)
	{
		Entity TestEntity = Registry.CreateEntity();

		Registry.AddComponent<ECSTestComponent>(TestEntity);
	}

	REQUIRE(Registry.GetAmountOfEntities() == 10);
}