//#include <catch.hpp>
//
//#include "../Memory/FreeListAllocator/FreeListAllocator.h"
//
//#include <string>
//
//__forceinline Integrian3D::Memory::FreeListAllocator& NewTestGetAllocator()
//{
//	static Integrian3D::Memory::FreeListAllocator allocator{ 4096 };
//
//	return allocator;
//}
//
//struct New_Test
//{
//	int m_A{ 16 };
//	bool m_B{ true };
//	std::string m_C{ "Hello World" };
//
//	//void* operator new(size_t n)
//	//{
//	//	return NewTestGetAllocator().Allocate(n, sizeof(uint64_t));
//	//}
//	//void* operator new[](size_t n)
//	//{
//	//	return NewTestGetAllocator().Allocate(n, sizeof(uint64_t));
//	//}
//
//	//	void operator delete(void* p)
//	//{
//	//	NewTestGetAllocator().Deallocate(p);
//	//}
//	//void operator delete[](void* p)
//	//{
//	//	NewTestGetAllocator().Deallocate(p);
//	//}
//};
//
//TEST_CASE("Testing the new & delete operator")
//{
//	int* pA{ new int{5} };
//
//	REQUIRE(pA != nullptr);
//	REQUIRE(*pA == 5);
//
//	int* pB{ new int{15} };
//
//	REQUIRE(pB != nullptr);
//	REQUIRE(*pB == 15);
//	REQUIRE(*pA != *pB);
//	REQUIRE(pA != pB);
//
//	__DELETE(pA);
//
//	REQUIRE(pA == nullptr);
//	REQUIRE(pB != nullptr);
//	REQUIRE(*pB == 15);
//
//	__DELETE(pB);
//
//	New_Test* pFoo{ new New_Test{} };
//
//	REQUIRE(pFoo != nullptr);
//	REQUIRE(pFoo->m_A == 16);
//	REQUIRE(pFoo->m_B == true);
//	REQUIRE(pFoo->m_C == "Hello World");
//
//	__DELETE(pFoo);
//	REQUIRE(pFoo == nullptr);
//
//	constexpr int nr{ 150 };
//	pFoo = new New_Test[nr]{};
//	REQUIRE(pFoo != nullptr);
//
//	for (int i{}; i < nr; ++i)
//	{
//		REQUIRE((pFoo + i)->m_A == 16);
//		REQUIRE((pFoo + i)->m_B == true);
//		REQUIRE((pFoo + i)->m_C == "Hello World");
//	}
//
//	__DELETEARR(pFoo);
//	REQUIRE(pFoo == nullptr);
//}