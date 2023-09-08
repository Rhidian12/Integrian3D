#include "Win32Utils/Win32APICall.h"

namespace Integrian3D::Win32Utils
{
	Win32APICallInfo::Win32APICallInfo(const std::string_view File, const int32 Line)
		: Win32APICallInfo(File, Line, 0)
	{}

	Win32APICallInfo::Win32APICallInfo(const std::string_view File, const int32 Line, const DWORD ErrorToIgnore)
		: Result{}
		, File{ File }
		, Line{ Line }
	{
		const DWORD Error{ GetLastError() };
		if (Error != ErrorToIgnore)
		{
			Result = Error;
		}

		SetLastError(ERROR_SUCCESS);
	}

	void Win32APICallInfo::LogError() const
	{
		if (GetSuccess())
		{
			return;
		}

		char* Buffer{};
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, Result, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char*)&Buffer, 0, nullptr);
		LOG(Win32, Error, "[%s, %i] Win32 API Call Error: %s", File.data(), Line, Buffer);
		LocalFree(Buffer);
	}

	bool Win32APICallInfo::GetSuccess() const
	{
		return Result == ERROR_SUCCESS;
	}
}