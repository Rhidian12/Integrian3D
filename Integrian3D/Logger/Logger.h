#pragma once

#include <memory>
#include <string_view>

namespace Integrian3D
{
	class Logger final
	{
	public:
		~Logger();

		static Logger& GetInstance();

		void LogMessage(
			const std::string_view Category,
			const std::string_view Visibility,
			const std::string_view Format,
			...
		);

		void AddLogCategory(const std::string_view Category);

	private:
		Logger();

		friend std::unique_ptr<Logger> std::make_unique();
		inline static std::unique_ptr<Logger> Instance{};

		struct LoggerStatics* Statics; // [TODO]: Use Custom Unique Ptr
	};
}

#define DECLARE_LOG_CATEGORY(Category) Integrian3D::Logger::GetInstance().AddLogCategory(Category);