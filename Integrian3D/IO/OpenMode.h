#pragma once

namespace Integrian3D::IO
{
	enum class OpenMode
	{
		CreateNew = 1,
		CreateAlways = 2,
		OpenExisting = 3,
		OpenAlways = 4,
		TruncateExisting = 5
	};
}