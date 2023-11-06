#pragma once

#include "../EngineConstants.h"

#include "TPair/TPair.h"

namespace Integrian3D
{
	template<typename K, typename V>
	TPair<K, V>::TPair()
		: m_Key{}
		, m_Value{}
	{}

	template<typename K, typename V>
	template<typename KK, typename VV>
	TPair<K, V>::TPair(KK&& key, VV&& val)
		: m_Key{ key }
		, m_Value{ val }
	{}

	template<typename K, typename V>
	TPair<K, V>::~TPair() {}

	template<typename K, typename V>
	TPair<K, V>::TPair(const TPair& Other) noexcept
		: m_Key{ Other.m_Key }
		, m_Value{ Other.m_Value }
	{}

	template<typename K, typename V>
	TPair<K, V>::TPair(TPair&& Other) noexcept
		: m_Key{ std::move(Other.m_Key) }
		, m_Value{ std::move(Other.m_Value) }
	{}

	template<typename K, typename V>
	TPair<K, V>& TPair<K, V>::operator=(const TPair& Other) noexcept
	{
		m_Key = Other.m_Key;
		m_Value = Other.m_Value;

		return *this;
	}

	template<typename K, typename V>
	TPair<K, V>& TPair<K, V>::operator=(TPair&& Other) noexcept
	{
		m_Key = std::move(Other.m_Key);
		m_Value = std::move(Other.m_Value);

		return *this;
	}

	template<typename K, typename V>
	K& TPair<K, V>::Key() &
	{
		return m_Key;
	}

	template<typename K, typename V>
	const K& TPair<K, V>::Key() const&
	{
		return m_Key;
	}

	template<typename K, typename V>
	K TPair<K, V>::Key() &&
	{
		return std::move(m_Key);
	}

	template<typename K, typename V>
	V& TPair<K, V>::Value() &
	{
		return m_Value;
	}

	template<typename K, typename V>
	const V& TPair<K, V>::Value() const&
	{
		return m_Value;
	}

	template<typename K, typename V>
	V TPair<K, V>::Value() &&
	{
		return m_Value;
	}
	
	template<typename K, typename V>
	__NODISCARD __INLINE TPair<std::remove_cvref_t<K>, std::remove_cvref_t<V>> MakePair(K&& key, V&& val)
	{
		return TPair<std::remove_cvref_t<K>, std::remove_cvref_t<V>>(key, val);
	}

	template<typename K, typename V>
	template<size_t I>
	auto& TPair<K, V>::get() &
	{
		static_assert(I < 2, "TPair::get<I>() > I must be smaller than 2");

		if constexpr (I == 0) return m_Key;
		else return m_Value;
	}

	template<typename K, typename V>
	template<size_t I>
	const auto& TPair<K, V>::get() const&
	{
		static_assert(I < 2, "TPair::get<I>() > I must be smaller than 2");

		if constexpr (I == 0) return m_Key;
		else return m_Value;
	}

	template<typename K, typename V>
	template<size_t I>
	auto TPair<K, V>::get() &&
	{
		static_assert(I < 2, "TPair::get<I>() > I must be smaller than 2");

		if constexpr (I == 0) return std::move(m_Key);
		else return std::move(m_Value);
	}
}

namespace std
{
	template<typename K, typename V> struct tuple_size<Integrian3D::TPair<K, V>> : public std::integral_constant<size_t, 2> {};

	template<typename K, typename V> struct tuple_element<0, Integrian3D::TPair<K, V>> { using type = K; };
	template<typename K, typename V> struct tuple_element<1, Integrian3D::TPair<K, V>> { using type = V; };
}