#include "BinaryReader.h"

#include "../../DebugUtility/DebugUtility.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace Integrian3D::IO
{
	BinaryReader::BinaryReader(const std::string& filepath)
		: m_Filepath{ filepath }
		, m_pHandle{}
		, m_Buffer{}
		, m_BufferPointer{}
	{
		/* open the file */
		m_pHandle = CreateFileA(filepath.c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			nullptr,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr);

		__ASSERT(m_pHandle != INVALID_HANDLE_VALUE && "BinaryReader could not open the provided file");

		/* Get the file size */
		const DWORD fileSize{ GetFileSize(m_pHandle, nullptr) };
		m_Buffer.Resize(fileSize);

		/* Read the file contents */
		DWORD readBytes{};
		if (ReadFile(m_pHandle, m_Buffer.Data(), fileSize, &readBytes, nullptr) == 0)
			Debug::LogError("BinaryReader could not read the provided file", false);
	}

	BinaryReader::~BinaryReader()
	{
		if (CloseHandle(m_pHandle) == 0)
			Debug::LogError("BinaryReader could not close the provided file", false);
	}
}