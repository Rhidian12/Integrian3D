#pragma once

namespace Integrian3D
{
	enum class LogVerbosity
	{
		// Logs are only printed if the Logger's current Verbosity Level is >= the requested Log Verbosity
		// e.g. if the Logger's level is 1, everything except 0 will be printed

		VeryVerbose		= 1,
		Verbose			= 0,
		MuteAll			= -1
	};
}