#include "Logger.h"

#include "../EngineConstants.h"
#include "LoggerStatics.h"
#include "ConsoleColours.h"

#include <iostream> /* std::cout */

#pragma warning ( push )
#pragma warning ( disable : 4005 ) /* warning C4005: 'APIENTRY': macro redefinition */ 
#	ifdef _WIN32
#		define WIN32_LEAN_AND_MEAN
#		include <Windows.h>
#	endif
#pragma warning ( pop )

namespace Integrian3D
{
	static constexpr LoggerStatics Statics{};

	namespace
	{
		static void SetConsoleColour(const Win32Utils::Win32Handle& ConsoleHandle, const std::string_view Visibility, const bool bIsDebug)
		{
#ifdef _WIN32
			// Debug is White by default, and so is any other Visibility that is not pre-defined
			WORD colour{ MessageColour::White };

			if (!bIsDebug)
			{
				if (Visibility == "Warning")
				{
					colour = MessageColour::Yellow;
				}
				else if (Visibility == "Error")
				{
					colour = MessageColour::LightRed;
				}
				else if (Visibility == "Fatal")
				{
					colour = MessageColour::Red;
				}
			}

			/* Set text colour to user defined colour */
			SetConsoleTextAttribute(ConsoleHandle, colour);
#endif
		}
	}

	Logger::Logger()
		: ConsoleHandle{ GetStdHandle(STD_OUTPUT_HANDLE) }
	{}

	Logger& Logger::GetInstance()
	{
		if (!Instance)
		{
			Instance = std::make_unique<Logger>();
		}

		return *Instance.get();
	}

	void Logger::LogMessage(
		const std::string_view Category,
		const std::string_view Visibility,
		const std::string_view Format,
		...)
	{
		SetConsoleColour(ConsoleHandle, Visibility, Visibility == "Debug");

		std::cout << "[" << Category << "]: ";

		va_list ArgPtr;
		va_start(ArgPtr, Format);

		vprintf(Format.data(), ArgPtr);

		va_end(ArgPtr);

		std::cout << "\n";

		SetConsoleColour(ConsoleHandle, "", true);
	}

	constexpr const LoggerStatics& GetLoggerStatics()
	{
		return Statics;
	}
}
