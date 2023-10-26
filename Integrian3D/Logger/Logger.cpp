#include "Logger.h"

#include "Array/Array.h"
#include "DebugUtility.h"
#include "Logger/ConsoleColours.h"
#include "Logger/LogCategory.h"
#include "IO/File/File.h"

#include <iostream>

#	ifdef _WIN32
	I_DISABLE_WARNING(4005) /* warning C4005: 'APIENTRY': macro redefinition */ 
#		define WIN32_LEAN_AND_MEAN
#		include <Windows.h>
	I_ENABLE_WARNING(4005)
#	endif

namespace Integrian3D
{
	namespace Detail
	{
		static void SetConsoleColour(const Win32Utils::Win32Handle& ConsoleHandle, const LogErrorLevel ErrorLevel)
		{
			#ifdef _WIN32
			// Log is White by default
			WORD colour{ MessageColour::White };

			switch (ErrorLevel)
			{
				case LogErrorLevel::Warning:
					colour = MessageColour::Yellow;
					break;
				case LogErrorLevel::Error:
					colour = MessageColour::LightRed;
					break;
				case LogErrorLevel::Fatal:
					colour = MessageColour::Red;
					break;
				default:
					break;
			}

			/* Set text colour to user defined colour */
			SetConsoleTextAttribute(ConsoleHandle, colour);
			#endif
		}
	}

	struct LoggerStatics
	{
		LoggerStatics()
			: Categories{}
			, File{ "Logs.txt", IO::OpenMode::CreateAlways, IO::FileMode::ASCII }
		{}

		TArray<LogCategory> Categories;
		IO::File File;

		const LogCategory* const GetLogCategory(const std::string_view CategoryName) const
		{
			const auto CIt{ Categories.Find([CategoryName](const LogCategory& LogCategory)->bool
				{
					return LogCategory.GetName() == CategoryName;
				}) };

			return CIt != Categories.cend() ? &(*CIt) : nullptr;
		}
	};

	Logger::Logger()
		: ConsoleHandle{ GetStdHandle(STD_OUTPUT_HANDLE) }
		, Statics{ new LoggerStatics{} }
		, VerbosityLevel{ LogVerbosity::Verbose }
	{}

	Logger::~Logger()
	{
		delete Statics;
		Statics = nullptr;
	}

	Logger& Logger::GetInstance()
	{
		if (!Instance)
		{
			Instance = new Logger{};
		}

		return *Instance;
	}

	void Logger::Cleanup()
	{
		if (Instance)
		{
			delete Instance;
			Instance = nullptr;
		}
	}

	void Logger::SetLogVerbosityLevel(const LogVerbosity NewVerbosity)
	{
		VerbosityLevel = NewVerbosity;
	}

	void Logger::AddCategory(const LogCategory& LogCategory)
	{
		Statics->Categories.Add(LogCategory);
	}

	const LogCategory* const Logger::GetLogCategory(const std::string_view RequestedLogCategory) const
	{
		return Statics->GetLogCategory(RequestedLogCategory);
	}

	bool Logger::CheckLogCategory(const LogCategory* const LogCategory, const std::string_view RequestedLogCategory) const
	{
		if (!LogCategory)
		{
			Detail::SetConsoleColour(ConsoleHandle, LogErrorLevel::Fatal);

			std::cout << std::format("Category {} was not added with DEFINE_CATEGORY()\n", RequestedLogCategory);

			ResetConsoleColour();

			__BREAK();

			return false;
		}

		if (LogCategory->GetVerbosity() == LogVerbosity::MuteAll || LogCategory->GetVerbosity() < VerbosityLevel)
		{
			return false;
		}

		return true;
	}

	void Logger::SetConsoleColour(const LogErrorLevel ErrorLevel) const
	{
		Detail::SetConsoleColour(ConsoleHandle, ErrorLevel);
	}

	void Logger::ResetConsoleColour() const
	{
		Detail::SetConsoleColour(ConsoleHandle, LogErrorLevel::Log);
	}

	void Logger::CheckErrorLevel(const LogErrorLevel ErrorLevel) const
	{
		if (ErrorLevel == LogErrorLevel::Fatal)
		{
			__BREAK();
		}
	}

	void Logger::WriteMessage(const std::string& Message) const
	{
		std::cout << Message;

		Statics->File << Message;
	}
}
