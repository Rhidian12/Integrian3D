#pragma once

#include "Logger/LogCategory.h"

#include <functional>

DEFINE_LOG_CATEGORY(Win32HandleLog, Integrian3D::LogVerbosity::Verbose);

namespace Integrian3D::Win32Utils
{
	class Win32Handle final
	{
		using Deleter = std::function<bool(void*)>;

	public:
		Win32Handle();
		explicit Win32Handle(void* const Handle);
		Win32Handle(void* const Handle, const Deleter& CustomDeleter);
		~Win32Handle();

		Win32Handle(const Win32Handle&) noexcept = delete;
		Win32Handle(Win32Handle&& Other) noexcept;
		Win32Handle& operator=(const Win32Handle&) noexcept = delete;
		Win32Handle& operator=(Win32Handle&& Other) noexcept;

		Win32Handle& operator=(const void* const Other) noexcept;

		bool IsValid() const;

		void* const Get();
		const void* const Get() const;

		operator void* () const;

	private:
		void CloseHandle();

		Deleter CustomDeleter;
		void* Handle;
	};
}