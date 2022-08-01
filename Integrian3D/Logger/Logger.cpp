#include "Logger.h"

#include <iostream> /* std::cout */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace Integrian3D
{
	namespace Debug
	{
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

		void Logger::LogMessage(const std::string_view message,
			[[maybe_unused]] const int lineNumber,
			[[maybe_unused]] const std::string_view file,
			const bool bVerbose)
		{
#ifdef _WIN32
			if (bVerbose)
			{
				std::cout << "A message was written from file: " << file.substr(file.find_last_of('\\') + 1, file.size()) <<
					" at line: " << std::to_string(lineNumber) << "\n" << "Message: " << message << "\n\n";
			}
			else
			{
				std::cout << message << "\n";
			}
#endif
		}

		void Logger::LogWarning(const std::string_view message,
			[[maybe_unused]] const int lineNumber,
			[[maybe_unused]] const std::string_view file,
			const bool bVerbose)
		{
#ifdef _WIN32
			/* Set text colour to yellow */
			SetConsoleTextAttribute(ConsoleHandle, static_cast<WORD>(MessageColour::Yellow));

			if (bVerbose)
			{
				std::cout << "A warning was written from file: " << file.substr(file.find_last_of('\\') + 1, file.size()) <<
					" at line: " << std::to_string(lineNumber) << "\n" << "Message: " << message << "\n\n";
			}
			else
			{
				std::cout << message << "\n";
			}

			/* Reset text colour to white */
			SetConsoleTextAttribute(ConsoleHandle, static_cast<WORD>(MessageColour::White));
#endif
		}

		void Logger::LogError(const std::string_view message,
			[[maybe_unused]] const int lineNumber,
			[[maybe_unused]] const std::string_view file,
			const bool bVerbose)
		{
#ifdef _WIN32
			/* Set text colour to red */
			SetConsoleTextAttribute(ConsoleHandle, static_cast<WORD>(MessageColour::Red));

			if (bVerbose)
			{
				std::cout << "An error was written from file: " << file.substr(file.find_last_of('\\') + 1, file.size()) <<
					" at line: " << std::to_string(lineNumber) << "\n" << "Message: " << message << "\n\n";
			}
			else
			{
				std::cout << message << "\n";
			}

			/* Reset text colour to white */
			SetConsoleTextAttribute(ConsoleHandle, static_cast<WORD>(MessageColour::White));
#endif
		}

		void Logger::LogCustomMessage(const std::string_view message,
			[[maybe_unused]] const int lineNumber,
			[[maybe_unused]] const std::string_view file,
			const MessageColour colour,
			const bool bVerbose)
		{
#ifdef _WIN32
			/* Set text colour to user defined colour */
			SetConsoleTextAttribute(ConsoleHandle, static_cast<WORD>(colour));

			if (bVerbose)
			{
				std::cout << "A custom message was written from file: " <<
					file.substr(file.find_last_of('\\') + 1, file.size()) <<
					" at line: " << std::to_string(lineNumber) << "\n" << "Custom Message: " << message << "\n\n";
			}
			else
			{
				std::cout << message << "\n";
			}

			/* Reset text colour to white */
			SetConsoleTextAttribute(ConsoleHandle, static_cast<WORD>(colour));
#endif
		}
	}
}
