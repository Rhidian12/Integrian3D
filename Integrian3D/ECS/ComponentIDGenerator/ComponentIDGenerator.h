#pragma once

#include "../../EngineConstants.h"
#include "../../Utils/Utils.h"

namespace Integrian3D
{
	template<typename T>
	constexpr ComponentType GenerateComponentID()
	{
		using namespace Utils;

		constexpr std::string_view typeName(ConstexprTypeName<T>());

		constexpr ComponentType hash(static_cast<ComponentType>(ConstexprStringHash(std::string{ typeName }.data(), std::string{ typeName }.size())));

		return hash;
	}
}