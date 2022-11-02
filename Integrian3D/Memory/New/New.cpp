#pragma once

#include "../../EngineConstants.h"
#include "../MemoryTracker/MemoryTracker.h"

void* operator new(size_t n)
{
	using namespace Integrian3D;

	Memory::MemoryTracker::Track(n);

	void* pData{ malloc(n) };

	__ASSERT(pData != nullptr && "Program ran out of memory");

	return pData;
}

void* operator new[](size_t n)
{
	using namespace Integrian3D;

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