#pragma once

#include "../EngineConstants.h"

namespace Integrian3D::TUtils
{
	template<typename ...>
	using Void = void;

	template<typename T>
	constexpr bool AlwaysFalse{ false };
}