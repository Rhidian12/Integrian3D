#include "Win32Utils/Win32Handle.h"
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
	Win32Handle::Win32Handle()
		: Handle{ INVALID_HANDLE_VALUE }
		, CustomDeleter{}
	{}

	Win32Handle::Win32Handle(void* const Handle)
		: Handle{ Handle }
		, CustomDeleter{}
	{}

	Win32Handle::Win32Handle(void* const Handle, const Deleter& CustomDeleter)
		: Handle{ Handle }
		, CustomDeleter{ CustomDeleter }
	{}

	Win32Handle::~Win32Handle()
	{
		CloseHandle();
	}

	Win32Handle::Win32Handle(Win32Handle&& Other) noexcept
		: Handle{ I_MOVE(Other.Handle) }
	{
		Other.Handle = INVALID_HANDLE_VALUE;
	}

	Win32Handle& Win32Handle::operator=(Win32Handle&& Other) noexcept
	{
		if (Handle)
		{
			CloseHandle();
		}

		Handle = I_MOVE(Other.Handle);
		Other.Handle = INVALID_HANDLE_VALUE;

		return *this;
	}

	Win32Handle& Win32Handle::operator=(const void* const Other) noexcept
	{
		if (Handle)
		{
			CloseHandle();
		}

		Handle = const_cast<void*>(Other);

		return *this;
	}

	bool Win32Handle::IsValid() const
	{
		return Handle != INVALID_HANDLE_VALUE;
	}

	void* const Win32Handle::Get()
	{
		return Handle;
	}

	const void* const Win32Handle::Get() const
	{
		return Handle;
	}

	Win32Handle::operator void* () const
	{
		return Handle;
	}

	void Win32Handle::CloseHandle()
	{
		if (Handle)
		{
			if (CustomDeleter)
			{
				if (!CustomDeleter(Handle))
				{
					LOG(Win32HandleLog, LogErrorLevel::Error, "Handle could not be closed through Custom Deleter");
				}
			}
			else
			{
				auto Call = CALL_WIN32(::CloseHandle(Handle));

				if (!Call.GetSuccess())
				{
					LOG(Win32HandleLog, LogErrorLevel::Error, "Handle could not be closed");
				}
			}

			Handle = INVALID_HANDLE_VALUE;
		}
	}
}