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
	namespace
	{
		static void SetConsoleColour(LoggerStatics* Statics, const std::string_view Visibility, const bool bIsDebug)
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
			SetConsoleTextAttribute(Statics->ConsoleHandle, colour);
#endif
		}
	}

	Logger::Logger()
		: Statics{ new LoggerStatics{} }
	{
		Statics->ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	Logger::~Logger()
	{
		if (Statics)
		{
			delete Statics;
			Statics = nullptr;
		}
	}

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
		SetConsoleColour(Statics, Visibility, Visibility == "Debug");

		std::cout << "[" << Category << "]: ";

		va_list ArgPtr;
		va_start(ArgPtr, Format);

		vprintf(Format.data(), ArgPtr);

		va_end(ArgPtr);

		std::cout << "\n";

		SetConsoleColour(Statics, "", true);
	}

	void Logger::AddLogCategory(const std::string_view Category)
	{
		__CHECK(Statics != nullptr);

		Statics->Categories.Add(Category);
	}
}
