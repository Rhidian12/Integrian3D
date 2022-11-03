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
		Start = new char[nrOfBytes] {};
		End = static_cast<char*>(Start) + nrOfBytes;
		Current = static_cast<char*>(Start);
	}

	LinearAllocator::~LinearAllocator()
	{
		delete[] Start;
	}

	LinearAllocator::LinearAllocator(const LinearAllocator& other) noexcept
		: Start{}
		, End{}
		, Current{}
	{
		Start = new char[other.Capacity()]{};
		End = static_cast<char*>(Start) + other.Capacity();
		Current = static_cast<char*>(Start);
	}

	LinearAllocator::LinearAllocator(LinearAllocator&& other) noexcept
		: Start{ __MOVE(void*, other.Start) }
		, End{ __MOVE(void*, other.End) }
		, Current{ __MOVE(void*, other.Current) }
	{
		other.Start = nullptr;
		other.End = nullptr;
		other.Current = nullptr;
	}

	LinearAllocator& LinearAllocator::operator=(const LinearAllocator& other) noexcept
	{
		if (Start)
		{
			delete[] Start;
		}

		Start = new char[other.Capacity()]{};
		End = static_cast<char*>(Start) + other.Capacity();
		Current = static_cast<char*>(Start);

		return *this;
	}
	LinearAllocator& LinearAllocator::operator=(LinearAllocator&& other) noexcept
	{
		if (Start)
		{
			delete[] Start;
		}

		Start = __MOVE(void*, other.Start);
		End = __MOVE(void*, other.End);
		Current = __MOVE(void*, other.Current);

		other.Start = nullptr;
		other.End = nullptr;
		other.Current = nullptr;

		return *this;
	}
}