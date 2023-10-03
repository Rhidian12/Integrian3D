#include "Win32Utils/Win32APICall.h"

#ifdef _WIN32
#	pragma warning ( push )
#	pragma warning ( disable : 4005 ) /* warning C4005: 'APIENTRY': macro redefinition */ 
#		define WIN32_LEAN_AND_MEAN
#		include <Windows.h>
#	pragma warning ( pop )
#endif

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
		LOG(Win32Log, LogErrorLevel::Error, "[{}, {}] Win32 API Call Error: {}", File, Line, Buffer);
		LocalFree(Buffer);
	}

	bool Win32APICallInfo::GetSuccess() const
	{
		return Result == ERROR_SUCCESS;
	}
}