#include "IO/PathUtils.h"


#ifdef _WIN32

#	include "Win32Utils/Win32APICall.h"

	I_DISABLE_WARNING(4005) // warning C4005: 'APIENTRY': macro redefinition
#		include <Windows.h>
	I_ENABLE_WARNING(4005)

#endif

namespace Integrian3D::PathUtils
{
	bool HasExtension(const std::string_view Path)
	{
		return Path.contains('.');
	}

	std::string_view GetExtension(const std::string_view Path)
	{
		if (!HasExtension(Path))
		{
			constexpr static std::string_view Empty = "";
			return Empty;
		}

		return Path.substr(Path.find_last_of('.'));
	}

	std::string_view GetPathWithoutExtension(const std::string_view Path)
	{
		if (!HasExtension(Path))
		{
			return Path;
		}
		else
		{
			return Path.substr(0, Path.find_last_of('.'));
		}
	}

	std::string_view GetCurrentDirectory()
	{
		static char Buffer[MAX_PATH]{ '\0' };
		if (Buffer[0] == '\0')
		{
			GetCurrentDirectoryA(MAX_PATH, Buffer);
		}

		return Buffer;
	}

	bool DoesFileExist(const std::string_view Path)
	{
		DWORD Attributes = CALL_WIN32_RV(GetFileAttributesA(Path.data()));

		return Attributes != INVALID_FILE_ATTRIBUTES && !(Attributes & FILE_ATTRIBUTE_DIRECTORY);
	}
}