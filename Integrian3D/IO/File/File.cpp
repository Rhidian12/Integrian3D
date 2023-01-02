#include "File.h"

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
	File::File()
		: m_Filepath{}
		, m_pHandle{}
		, m_Buffer{}
	{}

	File::File(const std::string& filepath)
		: m_Filepath{ filepath }
		, m_pHandle{}
		, m_Buffer{}
	{
		/* open the file */
		m_pHandle = CreateFileA(filepath.data(),
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ,
			nullptr,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr);

		if (m_pHandle != INVALID_HANDLE_VALUE)
			Debug::LogError("File could not open the provided file", false);

		/* Get the file size */
		const DWORD fileSize{ GetFileSize(m_pHandle, nullptr) };
		m_Buffer.Resize(fileSize);

		/* Read the file contents */
		DWORD readBytes{};
		if (ReadFile(m_pHandle, m_Buffer.Data(), fileSize, &readBytes, nullptr) == 0)
			Debug::LogError("File could not read the provided file", false);
	}

	File::~File()
	{
		if (m_pHandle)
			if (CloseHandle(m_pHandle) == 0)
				Debug::LogError("File could not close the provided file", false);
	}

	File::File(File&& other) noexcept
		: m_Filepath{ __MOVE(other.m_Filepath) }
		, m_pHandle{ __MOVE(other.m_pHandle) }
		, m_Buffer{ __MOVE(other.m_Buffer) }
	{
		other.m_pHandle = nullptr;
	}

	File& File::operator=(File&& other) noexcept
	{
		m_Filepath = __MOVE(other.m_Filepath);
		m_pHandle = __MOVE(other.m_pHandle);
		m_Buffer = __MOVE(other.m_Buffer);

		other.m_pHandle = nullptr;

		return *this;
	}

	void File::Write() const
	{
		if (WriteFile(m_pHandle, m_Buffer.Data(), static_cast<DWORD>(m_Buffer.Size()), nullptr, nullptr) == 0)
			Debug::LogError("BinaryWriter could not write the data to the provided file", false);
	}

	TArray<char>& File::GetBuffer()
	{
		return m_Buffer;
	}

	const TArray<char>& File::GetBuffer() const
	{
		return m_Buffer;
	}
}