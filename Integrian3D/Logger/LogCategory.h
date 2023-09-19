#pragma once

#include <string>

namespace Integrian3D
{
	enum class LogVerbosity
	{
		Log,
		Warning,
		Error,
		Fatal
	};

	class LogCategory final
	{
	public:
		explicit LogCategory(const std::string& Name, const LogVerbosity Verbosity)
			: Name{ Name }
			, Verbosity{ Verbosity }
		{}

		const std::string& GetName() const
		{
			return Name;
		}

		LogVerbosity GetVerbosity() const
		{
			return Verbosity;
		}

	private:
		std::string Name;
		LogVerbosity Verbosity;
	};
}

#define DEFINE_LOG_CATEGORY()