//#pragma once
//
//#include "../../EngineConstants.h"
//#include "../MemoryTracker/MemoryTracker.h"
//#include "../FreeListAllocator/FreeListAllocator.h"
//
//__INLINE Integrian3D::Memory::FreeListAllocator& GetAllocator()
//{
//	static Integrian3D::Memory::FreeListAllocator allocator{ 4096 };
//
//	return allocator;
//}
//
//void* operator new(size_t n)
//{
//	using namespace Integrian3D;
//
//	if (n == 0) /* Standard dictates that 0 byte requests must return a valid pointer */
//		n = 1; /* So, make the size 1 byte instead of 0 */
//
//	Memory::MemoryTracker::Track(n);
//
//	void* pData{ GetAllocator().Allocate(n, sizeof(uint64_t)) };
//
//	__ASSERT(pData != nullptr && "Program ran out of memory");
//
//	return pData;
//}
//
//void* operator new[](size_t n)
//{
//	using namespace Integrian3D;
//
//	if (n == 0) /* Standard dictates that 0 byte requests must return a valid pointer */
//		n = 1; /* So, make the size 1 byte instead of 0 */
//
//	Memory::MemoryTracker::Track(n);
//
//	void* pData{ GetAllocator().Allocate(n, sizeof(uint64_t)) };
//
//	__ASSERT(pData != nullptr && "Program ran out of memory");
//
//	return pData;
//}
//
//void operator delete(void* p)
//{
//	using namespace Integrian3D;
//
//	// Memory::MemoryTracker::StopTracking(n);
//
//	GetAllocator().Deallocate(p);
//
//	// free(p);
//}
//
//void operator delete[](void* p)
//{
//	using namespace Integrian3D;
//
//	// Memory::MemoryTracker::StopTracking(n);
//
//	GetAllocator().Deallocate(p);
//
//	// free(p);
//}