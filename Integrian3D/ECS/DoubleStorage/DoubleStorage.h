#pragma once

#include <algorithm> /* std::find */
#include <vector> /* std::vector */

namespace Integrian3D
{
	template<typename TKey, typename TValue>
	class DoubleStorage final
	{
	public:
		DoubleStorage() = default;

		TValue& Add(const TKey& key, const TValue& val)
		{
			Keys.push_back(key);
			Values.push_back(val);

			return Values.back();
		}
		TValue& Add(TKey&& key, TValue&& val)
		{
			Keys.push_back(std::move(key));
			Values.push_back(std::move(val));

			return Values.back();
		}

		void Remove(const TKey& key)
		{
			auto it = std::find(Keys.begin(), Keys.end(), key);
			auto index = it - Keys.begin();

			Keys.erase(it);
			Values.erase(Values.begin() + index);
		}

		TValue& GetValue(const TKey& key)
		{
			const auto index = std::find(Keys.begin(), Keys.end(), key) - Keys.begin();

			return Values[index];
		}
		const TValue& GetValue(const TKey& key) const
		{
			const auto index = std::find(Keys.cbegin(), Keys.cend(), key) - Keys.cbegin();

			return Values[index];
		}

		std::vector<TKey>& GetKeys() { return Keys; }
		const std::vector<TKey>& GetKeys() const { return Keys; }

		std::vector<TValue>& GetValues() { return Values; }
		const std::vector<TValue>& GetValues() const { return Values; }

	private:
		std::vector<TKey> Keys;
		std::vector<TValue> Values;
	};
}