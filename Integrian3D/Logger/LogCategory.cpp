#include "Logger/LogCategory.h"
#include "Logger/Logger.h"

namespace Integrian3D
{
	LogCategory::LogCategory(const std::string& Name, const LogVerbosity Verbosity)
		: Name{ Name }
		, Verbosity{ Verbosity }
	{
		Logger::GetInstance().AddCategory(*this);
	}

	const std::string& LogCategory::GetName() const
	{
		return Name;
	}

	LogVerbosity LogCategory::GetVerbosity() const
	{
		return Verbosity;
	}
}