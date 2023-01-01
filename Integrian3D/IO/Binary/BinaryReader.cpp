#include "BinaryReader.h"

#include "../../DebugUtility/DebugUtility.h"

#pragma warning ( push )
#pragma warning ( disable : 4005 ) /* warning C4005: 'APIENTRY': macro redefinition */ 
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
#pragma warning ( pop )

namespace Integrian3D::IO
{
	BinaryReader::BinaryReader()
		: m_pHandle{}
	{}

	BinaryReader::BinaryReader(const std::string_view filepath)
		: m_pHandle{}
		, m_Buffer{}
		, m_BufferPointer{}
	{
		/* open the file */
		m_pHandle = CreateFileA(filepath.data(),
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
		if (m_pHandle)
			if (CloseHandle(m_pHandle) == 0)
				Debug::LogError("BinaryReader could not close the provided file", false);
	}

	BinaryReader::BinaryReader(BinaryReader&& other) noexcept
		: m_pHandle{ __MOVE(other.m_pHandle) }
		, m_Buffer{ __MOVE(other.m_Buffer) }
		, m_BufferPointer{ __MOVE(other.m_BufferPointer) }
	{
		other.m_pHandle = nullptr;
	}

	BinaryReader& BinaryReader::operator=(BinaryReader&& other) noexcept
	{
		m_pHandle = __MOVE(other.m_pHandle);
		m_Buffer = __MOVE(other.m_Buffer);
		m_BufferPointer = __MOVE(other.m_BufferPointer);

		other.m_pHandle = nullptr;

		return *this;
	}
}