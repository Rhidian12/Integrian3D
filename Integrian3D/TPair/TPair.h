#pragma once

#include "../EngineConstants.h"

namespace Integrian3D
{
	template<typename K, typename V>
	class TPair final
	{
	public:
		TPair() {}
		template<typename KK = K, typename VV = V>
		TPair(KK&& key, VV&& val);

		K& Key() &;
		const K& Key() const&;
		K Key() &&;

		V& Value() &;
		const V& Value() const&;
		V Value() &&;

		template<size_t I>
		auto& get()&;

		template<size_t I>
		const auto& get() const&;

		template<size_t I>
		auto get() &&;

		// dont allow r-values to bind to const l-value overload
		K Key() const&& = delete;
		V Value() const&& = delete;

		template<size_t I>
		auto get() const&& = delete;

	private:
		K m_Key;
		V m_Value;
	};
}