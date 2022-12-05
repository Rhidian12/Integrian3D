#include "FileReader.h"

#include "../DebugUtility/DebugUtility.h"

#pragma warning( push )
#pragma warning( disable:4005 ) /* Macro redefinition */
/* Win32 and GLFW both define APIENTRY */
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
#pragma warning( pop )

#include <sstream> /* std::stringstream */

namespace Integrian3D
{
	FileReader::FileReader(const std::string& filePath, const char delimiter)
		: Handle{}
		, FilePath{ filePath }
		, FileContents{}
		, DelimitedFileContents{}
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
		FileContents.resize(fileSize);

		if (ReadFile(Handle, FileContents.data(), fileSize, &readBytes, nullptr) == 0)
		{
			Debug::LogError("FileReader could not read the provided file", false);
		}

		std::stringstream ss{ FileContents };
		std::string temp{};

		while (std::getline(ss, temp, delimiter))
		{
			DelimitedFileContents.push_back(temp);
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
		, FilePath{ __MOVE(other.FilePath) }
		, FileContents{ __MOVE(other.FileContents) }
		, DelimitedFileContents{ __MOVE(other.DelimitedFileContents) }
	{
		if (Handle)
		{
			if (CloseHandle(Handle) == 0)
			{
				Debug::LogError("FileReader could not close the provided file", false);
			}
		}

		Handle = __MOVE(other.Handle);
		other.Handle = nullptr;
	}

	FileReader& FileReader::operator=(FileReader&& other) noexcept
	{
		FilePath = __MOVE(other.FilePath);
		FileContents = __MOVE(other.FileContents);
		DelimitedFileContents = __MOVE(other.DelimitedFileContents);

		if (Handle)
		{
			if (CloseHandle(Handle) == 0)
			{
				Debug::LogError("FileReader could not close the provided file", false);
			}
		}

		Handle = __MOVE(other.Handle);
		other.Handle = nullptr;

		return *this;
	}
}