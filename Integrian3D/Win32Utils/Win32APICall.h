#pragma once
#include "EngineConstants.h"

#include <string>
#include <string_view>

#ifdef _WIN32
#	pragma warning ( push )
#	pragma warning ( disable : 4005 ) /* warning C4005: 'APIENTRY': macro redefinition */ 
#		define WIN32_LEAN_AND_MEAN
#		include <Windows.h>
#	pragma warning ( pop )
#endif

namespace Integrian3D::Win32Utils
{
	class Win32APICallInfo final
	{
	public:
		Win32APICallInfo(const bool bSuccess);

		void LogError();
		bool GetSuccess() const;

	private:
		bool bSuccess;
	};
}

#ifdef _WIN32
#	define CALL_WIN32_API(Function) Integrian3D::Win32Utils::Win32APICallInfo { Integrian3D::Win32Utils::CallWin32API(Function) };
#endif