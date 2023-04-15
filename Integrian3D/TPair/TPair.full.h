#pragma once

#include "../EngineConstants.h"

#include "TPair.h"

namespace Integrian3D
{
	template<typename K, typename V>
	template<typename KK, typename VV>
	TPair<K, V>::TPair(KK&& key, VV&& val)
		: m_Key{ key }
		, m_Value{ val }
	{}

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
}