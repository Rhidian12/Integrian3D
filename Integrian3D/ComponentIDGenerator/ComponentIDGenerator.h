#pragma once

#include "../EngineConstants.h"
#include "../Utils/Utils.h"

namespace Integrian3D
{
	template<typename T>
	constexpr ComponentType GenerateComponentID()
	{
		using namespace Utils;

		constexpr const char* typeName(ConstexprTypeName<T>());

		constexpr ComponentType hash(static_cast<ComponentType>(ConstexprStringHash(typeName)));

		return hash;
	}
}