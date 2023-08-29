#include <catch.hpp>

#include "../Memory/FreeListAllocator/FreeListAllocator.h"

#include <string>
#include <vector>

class Foo final
{
public:
	Foo()
		: m_Nr{}
		, m_Text{}
	{}
	Foo(const int nr, const std::string& text)
		: m_Nr{ nr }
		, m_Text{ text }
	{}

	int m_Nr;
	std::string m_Text;
};

TEST_CASE("Testing the FreeListAllocator")
{
	using namespace Integrian3D::Memory;

	FreeListAllocator alloc{ 16 };

	Foo* pFoo = static_cast<Foo*>(alloc.Allocate(sizeof(Foo), alignof(Foo)));
	alloc.Construct(pFoo, 15, "YAHOO");
	REQUIRE(pFoo != nullptr);
	REQUIRE(alloc.Capacity() > 16);
	REQUIRE(alloc.Size() == 1);

	REQUIRE(pFoo->m_Text == "YAHOO");
	REQUIRE(pFoo->m_Nr == 15);

	alloc.Destroy(pFoo);
	alloc.Deallocate(pFoo);

	REQUIRE(alloc.Capacity() > 16);
	REQUIRE(alloc.Size() == 0);

	// std::vector<Foo*> foos{};
	Foo* foos[6]{};

	for (int i{}; i < 6; ++i)
		// foos.push_back(alloc.Construct(static_cast<Foo*>(alloc.Allocate(sizeof(Foo), alignof(Foo))), 35, "SomeString"));
		foos[i] = alloc.Construct(static_cast<Foo*>(alloc.Allocate(sizeof(Foo), alignof(Foo))), 35, "SomeString");

	for (int i{}; i < 6; ++i)
	{
		alloc.Destroy(foos[i]);
		alloc.Deallocate(foos[i]);
	}
}