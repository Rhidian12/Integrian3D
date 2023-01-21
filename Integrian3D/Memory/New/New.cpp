#pragma once

#include "../../EngineConstants.h"
#include "../MemoryTracker/MemoryTracker.h"

void* operator new(size_t n)
{
	using namespace Integrian3D;

	if (n == 0) /* Standard dictates that 0 byte requests must return a valid pointer */
		n = 1; /* So, make the size 1 byte instead of 0 */

	Memory::MemoryTracker::Track(n);

	void* pData{ malloc(n) };

	__ASSERT(pData != nullptr && "Program ran out of memory");

	return pData;
}

void* operator new[](size_t n)
{
	using namespace Integrian3D;

	if (n == 0) /* Standard dictates that 0 byte requests must return a valid pointer */
		n = 1; /* So, make the size 1 byte instead of 0 */

	Memory::MemoryTracker::Track(n);

	void* pData{ malloc(n) };

	__ASSERT(pData != nullptr && "Program ran out of memory");

	return pData;
}

void operator delete(void* p, size_t n)
{
	using namespace Integrian3D;

	Memory::MemoryTracker::StopTracking(n);

	free(p);
}

void operator delete[](void* p, size_t n)
{
	using namespace Integrian3D;

	Memory::MemoryTracker::StopTracking(n);

	free(p);
}