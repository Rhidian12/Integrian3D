#include "FileReader.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include "../DebugUtility/DebugUtility.h"

namespace Integrian3D
{
	FileReader::FileReader(const std::string& filePath)
		: Handle{}
		, FilePath{ filePath }
	{
		/* open the file */
		Handle = CreateFileA(filePath.c_str(),
				GENERIC_READ,
				FILE_SHARE_READ,
				nullptr,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				nullptr);

		if (Handle == INVALID_HANDLE_VALUE)
		{
			Debug::LogError("FileReader could not open the provided file", false);
			return;
		}

		/* Get the file size */
		const DWORD fileSize{ GetFileSize(Handle, nullptr) };
		FileContents.resize(fileSize);

		/* Read the file contents */
		DWORD readBytes{};
		if (ReadFile(Handle, FileContents.data(), fileSize, &readBytes, nullptr) == 0)
		{
			Debug::LogError("FileReader could not read the provided file", false);
			return;
		}
	}

	FileReader::~FileReader()
	{
		if (CloseHandle(Handle) == 0)
		{
			Debug::LogError("FileReader could not close the provided file", false);
		}
	}
}