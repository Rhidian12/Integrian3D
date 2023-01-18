#pragma once

#include "../EngineConstants.h"
#include "../Math/Math.h"
#include "../TemplateUtils/TemplateUtils.h"

namespace Integrian3D::GOUtils
{
	namespace Detail
	{
		template<typename T>
		constexpr std::string_view WrappedTypeName()
		{
			return __FUNCSIG__;
		}
	}

	/* Reference for TypeName: https://stackoverflow.com/questions/35941045/can-i-obtain-c-type-names-in-a-constexpr-way */
	template <typename T>
	constexpr std::string_view ConstexprTypeName()
	{
		constexpr std::string_view wrappedName(Detail::WrappedTypeName<T>());

		constexpr size_t endOfType{ wrappedName.find_last_of('>') };
		constexpr size_t beginOfType{ Math::Max(wrappedName.find_last_of(' '), wrappedName.find_last_of('<')) };

		return wrappedName.substr(beginOfType + 1, endOfType - beginOfType - 1);
	}

	__INLINE constexpr uint32_t ConstexprStringHash(const char* pKey, size_t count)
	{
		uint32_t p = 31;
		const uint32_t m = static_cast<uint32_t>(1e9) + 9;
		uint32_t hash_value = 0;
		uint32_t p_pow = 1;
		const char* c = pKey;
		while (count > 0)
		{
			hash_value = (hash_value + (*c - static_cast<uint32_t>('a') + 1) * p_pow) % m;
			p_pow = (p_pow * p) % m;
			c++;
			--count;
		}

		return hash_value;
	}

	template<typename T>
	constexpr uint32_t GenerateComponentID()
	{
		constexpr std::string_view typeName(ConstexprTypeName<T>());

		constexpr uint32_t hash(ConstexprStringHash(std::string{ typeName }.data(), std::string{ typeName }.size()));

		return hash;
	}
}