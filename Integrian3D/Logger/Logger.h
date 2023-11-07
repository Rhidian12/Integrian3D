#pragma once

#include "Logger/LogVerbosity.h"
#include "Macros.h"
#include "Win32Utils/Win32Handle.h"

#include <format>
#include <mutex>
#include <string>
#include <string_view>

namespace Integrian3D
{
	class LogCategory;

	enum class LogErrorLevel
	{
		Log,
		Warning,
		Error,
		Fatal
	};

	class Logger final
	{
	public:
		~Logger();

		static Logger& GetInstance();
		static void Initialize();
		static void StartCleanup();

		void SetLogVerbosityLevel(const LogVerbosity NewVerbosity);

		template<typename ... Args>
		void LogMessage
		(
			const std::string_view Category,
			const LogErrorLevel ErrorLevel,
			const std::string_view Format,
			Args&& ... Arguments
		) const
		{
			const std::unique_lock<std::mutex> Lock{ Mutex };

			const LogCategory* const LogCategory{ GetLogCategory(Category) };

			if (!CheckLogCategory(LogCategory, Category))
			{
				return;
			}

			SetConsoleColour(ErrorLevel);

			WriteMessage(std::format("[{}] {}\n", Category, std::vformat(Format, std::make_format_args(__FORWARD(Arguments)...))));

			CheckErrorLevel(ErrorLevel);

			ResetConsoleColour(); // [TODO]: Make ON_SCOPE_EXIT macro
		}

	private:
		friend class LogCategory;
		void AddCategory(const class LogCategory& LogCategory);

		const LogCategory* const GetLogCategory(const std::string_view RequestedLogCategory) const;
		bool CheckLogCategory(const LogCategory* const LogCategory, const std::string_view RequestedLogCategory) const;
		void SetConsoleColour(const LogErrorLevel ErrorLevel) const;
		void ResetConsoleColour() const;
		void CheckErrorLevel(const LogErrorLevel ErrorLevel) const;
		void WriteMessage(const std::string& Message) const;

		Logger();

		struct LoggerStatics* Statics;
		Win32Utils::Win32Handle ConsoleHandle;
		mutable std::mutex Mutex;
		LogVerbosity VerbosityLevel; 
	};
}