#pragma once

#include "../../EngineConstants.h"

namespace Integrian3D::IO
{
	enum class SeekMode : uint8_t
	{
		Start = 0,
		End = 1,
		Advance = 2,
		Regress = 3
	};
}