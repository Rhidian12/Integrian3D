#pragma once

#include <iterator> /* std::iterator, ... */

namespace Integrian3D
{
	template
		<
		typename T,
		typename Type = std::bidirectional_iterator_tag,
		typename Diff = std::ptrdiff_t,
		typename = std::enable_if_t<std::is_base_of_v<std::input_iterator_tag, Type>>,
		typename = std::enable_if_t<std::is_integral_v<Diff>>
		>
		class Iterator final
	{
		using Pointer = T*;
		using Reference = T&;

	public:
		Iterator(Pointer _pPointer)
			: pPointer{ _pPointer }
		{
			static_assert(!std::is_same_v<Type, std::random_access_iterator_tag>, "Integrian3D::Iterator > This class does not support random access iterator!");
		}

		Reference operator*() const
		{
			return *pPointer;
		}

		Pointer operator->()
		{
			return pPointer;
		}

		Iterator& operator++()
		{
			++pPointer;
			return *this;
		}

		Iterator operator++(int)
		{
			Iterator tmp = *this;
			++(*this);
			return tmp;
		}

		Iterator& operator--()
		{
			static_assert(std::is_same_v<Type, std::bidirectional_iterator_tag>, "Iterator::operator--() > this operator is only supported on bidirectional iterators");

			--pPointer;
			return *this;
		}

		Iterator operator--(int)
		{
			static_assert(std::is_same_v<Type, std::bidirectional_iterator_tag>, "Iterator::operator--(int) > this operator is only supported on bidirectional iterators");

			Iterator tmp = *this;
			--(*this);
			return tmp;
		}

		bool operator==(const Iterator& other) const
		{
			return pPointer == other.pPointer;
		}

		bool operator!=(const Iterator& other) const
		{
			return pPointer != other.pPointer;
		}

	private:
		Pointer pPointer;
	};

	template
		<
		typename T,
		typename Type = std::bidirectional_iterator_tag,
		typename Diff = std::ptrdiff_t,
		typename = std::enable_if_t<std::is_base_of_v<std::input_iterator_tag, Type>>,
		typename = std::enable_if_t<std::is_integral_v<Diff>>
		>
		class ConstIterator final
	{
		using Pointer = const T*;
		using Reference = const T&;

	public:
		ConstIterator(Pointer _pPointer)
			: pPointer{ _pPointer }
		{
			static_assert(!std::is_same_v<Type, std::random_access_iterator_tag>, "Integrian3D::Iterator > This class does nut support random access iterator!");
		}

		Reference operator*() const
		{
			return *pPointer;
		}

		Pointer operator->()
		{
			return pPointer;
		}

		ConstIterator& operator++()
		{
			++pPointer;
			return *this;
		}

		ConstIterator operator++(int)
		{
			Iterator tmp = *this;
			++(*this);
			return tmp;
		}

		ConstIterator& operator--()
		{
			static_assert(std::is_same_v<Type, std::bidirectional_iterator_tag>, "ConstIterator::operator--() > this operator is only supported on bidirectional iterators");

			--pPointer;
			return *this;
		}

		ConstIterator operator--(int)
		{
			static_assert(std::is_same_v<Type, std::bidirectional_iterator_tag>, "ConstIterator::operator--(int) > this operator is only supported on bidirectional iterators");

			Iterator tmp = *this;
			--(*this);
			return tmp;
		}

		bool operator==(const ConstIterator& other) const
		{
			return pPointer == other.pPointer;
		}

		bool operator!=(const ConstIterator& other) const
		{
			return pPointer != other.pPointer;
		}

	private:
		Pointer pPointer;
	};
}