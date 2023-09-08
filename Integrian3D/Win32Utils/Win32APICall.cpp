#include "Win32/Win32APICall.h"

namespace Integrian3D::Win32Utils
{
	Win32APICallInfo::Win32APICallInfo(const bool bSuccess)
		: bSuccess{ bSuccess }
	{}

	void Win32APICallInfo::LogError()
	{
		if (bSuccess)
		{
			return;
		}

		char* Buffer{};
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), Buffer, 0, nullptr);
		LOG(Win32, Error, "Win32 API Call Error: %s", Buffer);
		LocalFree(Buffer);
	}

	bool Win32APICallInfo::GetSuccess() const
	{
		return bSuccess;
	}
}