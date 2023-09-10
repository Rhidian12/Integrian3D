#pragma once

#include "Win32Utils/Win32Handle.h"

#include <memory>
#include <string_view>

namespace Integrian3D
{
	class Logger final
	{
	public:
		~Logger() = default;

		static Logger& GetInstance();

		void LogMessage(
			const std::string_view Category,
			const std::string_view Visibility,
			const std::string_view Format,
			...
		);

	private:
		Logger();

		friend std::unique_ptr<Logger> std::make_unique();
		inline static std::unique_ptr<Logger> Instance{};

		Win32Utils::Win32Handle ConsoleHandle;
	};

	constexpr const struct LoggerStatics& GetLoggerStatics();

	template<typename T>
	constexpr bool CheckLogCategory(const T);

	template<>
	constexpr bool CheckLogCategory<const char*>(const char* Category)
	{
		return Statics.Categories.Contains(Category);
	}
}

#define DECLARE_LOG_CATEGORY(Category) Integrian3D::Statics.Categories.AddUnique(Category);