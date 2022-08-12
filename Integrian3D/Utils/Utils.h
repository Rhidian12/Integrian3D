#pragma once

#include <string_view> /* std::string_view */
#include <limits> /* std::numeric_limits */

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
			//constexpr std::string_view wrappedName(WrappedTypeName<T>());
			//constexpr std::string_view wrappedVoidName(WrappedTypeName<void>());
			//constexpr std::string_view voidName(ConstexprTypeName<void>());

			//constexpr size_t prefixLength(wrappedVoidName.find(voidName));
			//constexpr size_t suffixLength(wrappedVoidName.length() - prefixLength - voidName.length());

			//constexpr size_t typeNameLength(wrappedVoidName.length() - prefixLength - suffixLength);
			//return wrappedName.substr(prefixLength, typeNameLength);

			constexpr std::string_view wrappedName(WrappedTypeName<T>());

			constexpr size_t endOfType{ wrappedName.find_last_of('>') };
			constexpr size_t beginOfType{ std::max(wrappedName.find_last_of(' '), wrappedName.find_last_of('<')) };

			return wrappedName.substr(beginOfType + 1, endOfType - beginOfType - 1);
		}

		__forceinline constexpr uint32_t ConstexprStringHash(const char* pKey, size_t count)
		{
			uint32_t p = 31;
			const uint32_t m = (uint32_t)1e9 + 9;
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
	}
}