#pragma once

#include "../EngineConstants.h"

#include <iterator> /* std::iterator, ... */

namespace Integrian3D
{
	enum class IteratorTag : uint8_t
	{
		ForwardIt			= 0,
		BidirectionalIt		= 1,
		RandomAccessIt		= 2
	};

	template<typename T, IteratorTag Tag, typename Diff = uint64_t>
	class Iterator final
	{
		using Pointer = T*;
		using Reference = T&;

	public:
		Iterator(Pointer _pPointer)
			: pPointer{ _pPointer }
		{}

#pragma region Access Data
		Reference operator*() const
		{
			return *pPointer;
		}

		Pointer operator->() const
		{
			return pPointer;
		}
#pragma endregion

#pragma region Arithmetic
		Iterator& operator++()
		{
			++pPointer;
			return *this;
		}

		Iterator& operator--()
		{
			static_assert(Tag > IteratorTag::ForwardIt, "Iterator::operator--() > This operator is only available for Bidirectional or Random Access iterators");

			--pPointer;
			return *this;
		}

		Iterator& operator+=(const uint64_t i)
		{
			pPointer += i;
			return *this;
		}

		Iterator& operator-=(const uint64_t i)
		{
			pPointer -= i;
			return *this;
		}

		Iterator operator++(int)
		{
			Iterator tmp = *this;
			++(*this);
			return tmp;
		}

		Iterator operator--(int)
		{
			static_assert(Tag > IteratorTag::ForwardIt, "Iterator::operator--() > This operator is only available for Bidirectional or Random Access iterators");

			Iterator tmp = *this;
			--(*this);
			return tmp;
		}

		Iterator operator+(const uint64_t i) const
		{
			return Iterator{ pPointer + i };
		}

		Iterator operator+(const Iterator& it) const
		{
			return Iterator{ pPointer + it.pPointer };
		}

		Iterator operator-(const uint64_t i) const
		{
			return Iterator{ pPointer - i };
		}

		Iterator operator-(const Iterator& it) const
		{
			return Iterator{ pPointer - it.pPointer };
		}
#pragma endregion

#pragma region Comparing Iterators
		bool operator==(const Iterator& other) const
		{
			return pPointer == other.pPointer;
		}

		bool operator!=(const Iterator& other) const
		{
			return pPointer != other.pPointer;
		}

		bool operator>(const Iterator& other) const
		{
			return pPointer > other.pPointer;
		}

		bool operator<(const Iterator& other) const
		{
			return pPointer < other.pPointer;
		}

		bool operator>=(const Iterator& other) const
		{
			return pPointer >= other.pPointer;
		}

		bool operator<=(const Iterator& other) const
		{
			return pPointer <= other.pPointer;
		}
#pragma endregion

	private:
		Pointer pPointer;
	};

	template<typename T, IteratorTag Tag, typename Diff = uint64_t>
	class ConstIterator final
	{
		using Pointer = const T*;
		using Reference = const T&;

	public:
		ConstIterator(Pointer _pPointer)
			: pPointer{ _pPointer }
		{}

#pragma region Access Data
		Reference operator*() const
		{
			return *pPointer;
		}

		Pointer operator->() const
		{
			return pPointer;
		}
#pragma endregion

#pragma region Arithmetic
		Iterator& operator++()
		{
			++pPointer;
			return *this;
		}

		Iterator& operator--()
		{
			static_assert(Tag > IteratorTag::ForwardIt, "Iterator::operator--() > This operator is only available for Bidirectional or Random Access iterators");

			--pPointer;
			return *this;
		}

		Iterator& operator+=(const uint64_t i)
		{
			pPointer += i;
			return *this;
		}

		Iterator& operator-=(const uint64_t i)
		{
			pPointer -= i;
			return *this;
		}

		Iterator operator++(int)
		{
			Iterator tmp = *this;
			++(*this);
			return tmp;
		}

		Iterator operator--(int)
		{
			static_assert(Tag > IteratorTag::ForwardIt, "Iterator::operator--() > This operator is only available for Bidirectional or Random Access iterators");

			Iterator tmp = *this;
			--(*this);
			return tmp;
		}

		Iterator operator+(const uint64_t i) const
		{
			return Iterator{ pPointer + i };
		}

		Iterator operator+(const Iterator& it) const
		{
			return Iterator{ pPointer + it.pPointer };
		}

		Iterator operator-(const uint64_t i) const
		{
			return Iterator{ pPointer - i };
		}

		Iterator operator-(const Iterator& it) const
		{
			return Iterator{ pPointer - it.pPointer };
		}
#pragma endregion

#pragma region Comparing Iterators
		bool operator==(const Iterator& other) const
		{
			return pPointer == other.pPointer;
		}

		bool operator!=(const Iterator& other) const
		{
			return pPointer != other.pPointer;
		}

		bool operator>(const Iterator& other) const
		{
			return pPointer > other.pPointer;
		}

		bool operator<(const Iterator& other) const
		{
			return pPointer < other.pPointer;
		}

		bool operator>=(const Iterator& other) const
		{
			return pPointer >= other.pPointer;
		}

		bool operator<=(const Iterator& other) const
		{
			return pPointer <= other.pPointer;
		}
#pragma endregion

	private:
		Pointer pPointer;
	};
}