#pragma once

#include "../EngineConstants.h"
#include "../Math/MathUtils.h"

#include <string_view> /* std::string_view */
#include <type_traits> /* std::true_type, ... */

namespace Integrian3D
{
	namespace Utils
	{
		/* Reference for TypeName: https://stackoverflow.com/questions/35941045/can-i-obtain-c-type-names-in-a-constexpr-way */

		template<typename T>
		constexpr std::string_view ConstexprTypeName();

		template<>
		constexpr std::string_view ConstexprTypeName<void>() { return "void"; }

		/* Should have internal linkage and therefore be unavailable to other files */
		namespace
		{
			template<typename Type>
			constexpr std::string_view WrappedTypeName()
			{
				return __FUNCSIG__;
			}
		}

		template <typename T>
		constexpr std::string_view ConstexprTypeName()
		{
			constexpr std::string_view wrappedName(WrappedTypeName<T>());

			constexpr size_t endOfType{ wrappedName.find_last_of('>') };
			constexpr size_t beginOfType{ MathUtils::Max(wrappedName.find_last_of(' '), wrappedName.find_last_of('<')) };

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

		template <typename T, template <typename ...> typename U>
		struct Is_Specialization : std::false_type {};

		template <template <typename ...> typename T, typename ... Ts>
		struct Is_Specialization<T<Ts...>, T> : std::true_type {};
	}
}