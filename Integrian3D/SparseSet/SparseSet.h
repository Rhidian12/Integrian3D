#pragma once

#include "../EngineConstants.h"
#include "../Iterator/Iterator.h"

#include <vector> /* std::vector */

namespace Integrian3D
{
	template<typename T>
	class SparseSet final
	{
	public:
		SparseSet()
			: Sparse{}
			, Packed{}
			, _Size{}
		{
			static_assert(std::is_integral_v<T>, "SparseSet only supports integer types");
		}

		/* Default Rule of 5 is sufficient */

		bool Add(const T& value)
		{
			return AddImpl(value);
		}
		bool Add(T&& value)
		{
			return AddImpl(__MOVE(value));
		}

		__NODISCARD bool Contains(const T value) const { return (value < Sparse.size()) && (Sparse[value] != InvalidEntityID); }
		__NODISCARD T GetIndex(const T value) const { __ASSERT(Contains(value) && "SparseSet::GetIndex() > The given value is not present in the Set"); return Sparse[value]; }

		__NODISCARD size_t Size() const { return _Size; }
		void Clear() { Sparse.clear(); Packed.clear(); _Size = 0; }

		bool Remove(const T value)
		{
			if (Contains(value))
			{
				--_Size;

				Sparse[value] = InvalidEntityID;

				return true;
			}

			return false;
		}

		void Reserve(const size_t capacity) { Sparse.reserve(capacity); Packed.reserve(capacity); }

		__NODISCARD T& operator[](const size_t index) { __ASSERT(index < _Size); return Packed[index]; }
		__NODISCARD const T operator[](const size_t index) const { __ASSERT(index < _Size); return Packed[index]; }

		Iterator<T, IteratorTag::RandomAccessIt> begin() { return Packed.data(); }
		ConstIterator<T, IteratorTag::RandomAccessIt> begin() const { return Packed.data(); }

		Iterator<T, IteratorTag::RandomAccessIt> end() { return Packed.data() + Packed.size(); }
		ConstIterator<T, IteratorTag::RandomAccessIt> end() const { return Packed.data() + Packed.size(); }

		ConstIterator<T, IteratorTag::RandomAccessIt> cbegin() const { return Packed.data(); }
		ConstIterator<T, IteratorTag::RandomAccessIt> cend() const { return Packed.data() + Packed.size(); }

	private:
		template<typename U>
		bool AddImpl(U&& value)
		{
			if (Contains(value))
			{
				return false;
			}

			if (Packed.size() <= _Size)
			{
				Packed.resize(_Size + 1);
			}
			if (Sparse.size() <= value)
			{
				Sparse.resize(value + 1, InvalidEntityID);
			}

			Packed[_Size] = value;
			Sparse[value] = _Size++;

			return true;
		}

		std::vector<T> Sparse;
		std::vector<T> Packed;
		T _Size;
	};
}