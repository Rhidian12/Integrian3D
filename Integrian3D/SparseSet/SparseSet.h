#pragma once

#include "../EngineConstants.h"

#include <vector> 

namespace Integrian3D
{
	template<typename T>
	class SparseSet final
	{
	private:
		using VectorIt = std::vector<T>::iterator;
		using VectorCIt = std::vector<T>::const_iterator;

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
			return AddImpl(std::move(value));
		}

		bool Contains(const T value) const
		{
			return (value < Sparse.size()) && (Sparse[value] != InvalidEntityID);
		}

		size_t Size() const { return _Size; }
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

		__NODISCARD __INLINE T& operator[](const T Val)
		{
			__ASSERT(Contains(Val), "SparseSet::operator[] > does not contain %i", Val);
			return Packed[Sparse[Val]];
		}
		__NODISCARD __INLINE const T operator[](const T Val) const
		{
			__ASSERT(Contains(Val), "SparseSet::operator[] > does not contain %i", Val);
			return Packed[Sparse[Val]];
		}

		VectorIt begin() { return Packed.begin(); }
		VectorCIt begin() const { return Packed.begin(); }

		VectorIt end() { return Packed.end(); }
		VectorCIt end() const { return Packed.end(); }

		VectorCIt cbegin() const { return Packed.cbegin(); }
		VectorCIt cend() const { return Packed.cend(); }

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