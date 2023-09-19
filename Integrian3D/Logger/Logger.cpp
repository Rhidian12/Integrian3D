#include "Logger.h"

#include "Array/Array.h"
#include "DebugUtility/DebugUtility.h"
#include "Logger/ConsoleColours.h"
#include "Logger/LogCategory.h"

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

	struct LoggerStatics
	{
		TArray<LogCategory> Categories;
	};

	Logger::Logger()
		: ConsoleHandle{ GetStdHandle(STD_OUTPUT_HANDLE) }
		, Statics{ MakeUnique<LoggerStatics>() }
	{}

	Logger& Logger::GetInstance()
	{
		if (!Instance)
		{
			Instance = MakeUnique<Logger>();
		}

		return *Instance.Get();
	}

	void Logger::LogMessage(
		const LogCategory& Category,
		const std::string_view Visibility,
		const std::string_view Format,
		...)
	{
		SetConsoleColour(ConsoleHandle, Visibility, Visibility == "Debug");

		std::cout << "[" << Category.GetName() << "] ";

		va_list ArgPtr;
		va_start(ArgPtr, Format);

		vprintf(Format.data(), ArgPtr);

		va_end(ArgPtr);

		std::cout << "\n";

		SetConsoleColour(ConsoleHandle, "", true);
	}
}
