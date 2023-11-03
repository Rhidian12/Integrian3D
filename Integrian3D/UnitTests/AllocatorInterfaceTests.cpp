//#include <catch.hpp>
//
//#include "../Memory/Allocator/Allocator.h"
//#include "../Memory/FreeListAllocator/FreeListAllocator.h"
//
//TEST_CASE("Testing the Allocator Interface")
//{
//	using namespace Integrian3D::Memory;
//
//	FreeListAllocator alloc{ 36 };
//	Allocator<FreeListAllocator> allocInterface{ std::move(alloc) };
//
//	auto a = allocInterface.Allocate<int>(1);
//	*a = 5;
//	REQUIRE(*a == 5);
//	REQUIRE(allocInterface.GetAllocator().Size() == 1);
//
//	auto b = allocInterface.Allocate<int>(1);
//	*b = 15;
//	REQUIRE(*a == 5);
//	REQUIRE(*b == 15);
//	REQUIRE(allocInterface.GetAllocator().Size() == 2);
//
//	auto c = allocInterface.Allocate<int>(50);
//	REQUIRE(*a == 5);
//	REQUIRE(*b == 15);
//	REQUIRE(allocInterface.GetAllocator().Size() == 3);
//
//	allocInterface.Deallocate(a);
//	REQUIRE(*b == 15);
//	REQUIRE(allocInterface.GetAllocator().Size() == 2);
//
//	auto d = allocInterface.Allocate<int>(1);
//	*d = 25;
//	REQUIRE(*b == 15);
//	REQUIRE(*d == 25);
//	REQUIRE(allocInterface.GetAllocator().Size() == 3);
//
//	allocInterface.Deallocate(c);
//	REQUIRE(*b == 15);
//	REQUIRE(*d == 25);
//	REQUIRE(allocInterface.GetAllocator().Size() == 2);
//
//	auto e = allocInterface.Allocate<double>(1);
//	*e = 36.0;
//	REQUIRE(*b == 15);
//	REQUIRE(*d == 25);
//	REQUIRE(*e == 36.0);
//	REQUIRE(allocInterface.GetAllocator().Size() == 3);
//
//	auto f = allocInterface.Allocate<int>(50);
//	REQUIRE(*b == 15);
//	REQUIRE(*d == 25);
//	REQUIRE(*e == 36.0);
//	REQUIRE(allocInterface.GetAllocator().Size() == 4);
//
//	for (int i{}; i < 50; ++i)
//		*(f + i) = i;
//
//	for (int i{}; i < 50; ++i)
//		REQUIRE(f[i] == i);
//
//	allocInterface.Deallocate(f);
//	REQUIRE(allocInterface.GetAllocator().Size() == 3);
//}