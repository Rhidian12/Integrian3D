#pragma once

#include "../EngineConstants.h"

#include "../Math/Math.h"

#include <string_view>

namespace Integrian3D::ECS
{
	namespace Detail
	{
		template<typename T>
		__NODISCARD static consteval std::string_view WrappedTypeName()
		{
			return __FUNCSIG__; // [TODO]: Make this cross-platform friendly
		}

		/* Reference for TypeName: https://stackoverflow.com/questions/35941045/can-i-obtain-c-type-names-in-a-constexpr-way */
		template <typename T>
		__NODISCARD static consteval std::string_view ConstexprTypeName()
		{
			constexpr std::string_view wrappedName(WrappedTypeName<T>());

			constexpr size_t endOfType{ wrappedName.find_last_of('>') };
			constexpr size_t beginOfType{ Math::Max(wrappedName.find_last_of(' '), wrappedName.find_last_of('<')) };

			return wrappedName.substr(beginOfType + 1, endOfType - beginOfType - 1);
		}

		__NODISCARD inline static consteval uint32_t ConstexprStringHash(const char* pKey, size_t count)
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
	}

	template<typename T>
	__NODISCARD static consteval size_t GenerateComponentID()
	{
		constexpr std::string_view typeName(Detail::ConstexprTypeName<T>());

		constexpr size_t hash
		{
			static_cast<size_t>(Detail::ConstexprStringHash(typeName.data(), typeName.size()))
		};

		return hash;
	}
}