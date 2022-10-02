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

		__ASSERT(Handle != INVALID_HANDLE_VALUE && "FileReader could not open the provided file");

		/* Get the file size */
		const DWORD fileSize{ GetFileSize(Handle, nullptr) };
		FileContents.resize(fileSize);

		/* Read the file contents */
		DWORD readBytes{};
		if (ReadFile(Handle, FileContents.data(), fileSize, &readBytes, nullptr) == 0)
		{
			Debug::LogError("FileReader could not read the provided file", false);
		}
	}

	FileReader::~FileReader()
	{
		if (CloseHandle(Handle) == 0)
		{
			Debug::LogError("FileReader could not close the provided file", false);
		}
	}

	FileReader::FileReader(FileReader&& other) noexcept
		: Handle{}
		, FilePath{ __MOVE(std::string, other.FilePath) }
		, FileContents{ __MOVE(std::string, other.FileContents) }
	{
		if (Handle)
		{
			if (CloseHandle(Handle) == 0)
			{
				Debug::LogError("FileReader could not close the provided file", false);
			}
		}

		Handle = __MOVE(void*, other.Handle);
		other.Handle = nullptr;
	}

	FileReader& FileReader::operator=(FileReader&& other) noexcept
	{
		FilePath = __MOVE(std::string, other.FilePath);
		FileContents = __MOVE(std::string, other.FileContents);

		if (Handle)
		{
			if (CloseHandle(Handle) == 0)
			{
				Debug::LogError("FileReader could not close the provided file", false);
			}
		}

		Handle = __MOVE(void*, other.Handle);
		other.Handle = nullptr;

		return *this;
	}
}