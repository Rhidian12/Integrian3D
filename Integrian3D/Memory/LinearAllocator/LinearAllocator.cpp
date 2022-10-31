#include "LinearAllocator.h"

namespace Integrian3D::Memory
{
	LinearAllocator::LinearAllocator()
		: LinearAllocator{ 1024 }
	{}

	LinearAllocator::LinearAllocator(const uint64_t nrOfBytes)
		: Start{}
		, End{}
		, Current{}
	{
		Start = malloc(nrOfBytes);
		End = static_cast<char*>(Start) + nrOfBytes;
		Current = static_cast<char*>(Start);
	}

	LinearAllocator::~LinearAllocator()
	{
		free(Start);
	}
}