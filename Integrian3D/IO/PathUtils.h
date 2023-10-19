#pragma once

#include "EngineConstants.h"

#include <string_view>

namespace Integrian3D::PathUtils
{
	bool HasExtension(const std::string_view Path);

	// returns extension, if present, including '.'
	// example: Hello_World.txt -> .txt
	std::string_view GetExtension(const std::string_view Path);

	std::string_view GetPathWithoutExtension(const std::string_view Path);
}