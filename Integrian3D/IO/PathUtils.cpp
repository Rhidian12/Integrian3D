#include "IO/PathUtils.h"

namespace Integrian3D::PathUtils
{
	bool HasExtension(const std::string_view Path)
	{
		return Path.contains('.');
	}

	std::string_view GetExtension(const std::string_view Path)
	{
		if (!HasExtension(Path))
		{
			constexpr static std::string_view Empty = "";
			return Empty;
		}

		return Path.substr(Path.find_last_of('.'));
	}

	std::string_view GetPathWithoutExtension(const std::string_view Path)
	{
		if (!HasExtension)
		{
			return Path;
		}
		else
		{
			return Path.substr(0, Path.find_last_of('.'));
		}
	}
}