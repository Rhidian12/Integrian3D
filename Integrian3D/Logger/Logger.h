#pragma once

#include "Macros/Macros.h"
#include "Memory/UniquePtr.h"
#include "TemplateUtils/TemplateUtils.h"
#include "Win32Utils/Win32Handle.h"

#include <string_view>

namespace Integrian3D
{
	struct LoggerStatics;
	class LogCategory;

	class Logger final
	{
	public:
		~Logger() = default;

		static Logger& GetInstance();

		void LogMessage(
			const LogCategory& Category,
			const std::string_view Visibility,
			const std::string_view Format,
			...
		);

	private:
		friend class LogCategory;

		Logger();

		friend UniquePtr<Logger> MakeUnique();
		inline static UniquePtr<Logger> Instance{};

		UniquePtr<LoggerStatics> Statics;
		Win32Utils::Win32Handle ConsoleHandle;
	};
}

#define LOG_INTERNAL(Category, Visibility, Format, ...) Integrian3D::Logger::GetInstance().LogMessage(#Category, #Visibility, Format, __VA_ARGS__)