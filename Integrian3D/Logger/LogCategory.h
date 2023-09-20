#pragma once

#include "Logger/LogVerbosity.h"

#include <string>

namespace Integrian3D
{
	class LogCategory final
	{
	public:
		explicit LogCategory(const std::string& Name, const LogVerbosity Verbosity);

		const std::string& GetName() const;
		LogVerbosity GetVerbosity() const;

	private:
		std::string Name;
		LogVerbosity Verbosity;
	};
}

#define DEFINE_LOG_CATEGORY(Name, Verbosity) inline static Integrian3D::LogCategory Name{ #Name, Verbosity };