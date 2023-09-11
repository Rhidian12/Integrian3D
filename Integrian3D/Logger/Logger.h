#pragma once

#include "Logger/LogVisibility.h"
#include "Macros/Macros.h"
#include "TemplateUtils/TemplateUtils.h"
#include "Win32Utils/Win32Handle.h"

#include <memory>
#include <string_view>

namespace Integrian3D
{
	struct LoggerStatics;

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

	namespace Detail
	{
		template<typename T, typename = void>
		struct __LogCategoryCheck : std::false_type {};

		template<typename T>
		struct __LogCategoryCheck<T, TUtils::Void<T>> : std::true_type {};

		template<typename T>
		inline constexpr void CheckLogCategory()
		{
			static_assert(__LogCategoryCheck<T>::value, "Log Category was not defined with DECLARE_LOG_CATEGORY");
		}
	}
}

#define LOG_INTERNAL(Category, Visibility, Format, ...) Integrian3D::Detail::CheckLogCategory<__LogCategory##Category>(); \
														Integrian3D::Logger::GetInstance().LogMessage(#Category, #Visibility, Format, __VA_ARGS__)