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
		, m_BufferPointer{}
	{}

	File::File(const std::string& filepath)
		: m_Filepath{ filepath }
		, m_pHandle{}
		, m_Buffer{}
		, m_BufferPointer{}
	{
		/* open the file */
		m_pHandle = CreateFileA(filepath.data(),
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ,
			nullptr,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			nullptr);

		if (m_pHandle == INVALID_HANDLE_VALUE)
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
		, m_BufferPointer{ __MOVE(other.m_BufferPointer) }
	{
		other.m_pHandle = nullptr;
	}

	File& File::operator=(File&& other) noexcept
	{
		m_Filepath = __MOVE(other.m_Filepath);
		m_pHandle = __MOVE(other.m_pHandle);
		m_Buffer = __MOVE(other.m_Buffer);
		m_BufferPointer = __MOVE(other.m_BufferPointer);

		other.m_pHandle = nullptr;

		return *this;
	}

	void File::Write() const
	{
		if (SetFilePointer(m_pHandle, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
			Debug::LogError("File::Write() > File could not set the file pointer", false);

		if (SetEndOfFile(m_pHandle) == 0)
			Debug::LogError("File::Write() > File could not be truncated", false);

		if (WriteFile(m_pHandle, m_Buffer.Data(), static_cast<DWORD>(m_Buffer.Size()), nullptr, nullptr) == 0)
			Debug::LogError("File::Write() > File could not write the data to the provided file", false);
	}

	void File::Write(const TArray<char>& data, const SeekMode mode)
	{
		__ASSERT(mode != SeekMode::Regress && "File::Write(const TArray&) > Cannot write while regressing");

		switch (mode)
		{
			case SeekMode::Start:
				for (int32_t i{ static_cast<int32_t>(data.Size()) - 1 }; i >= 0; --i)
					m_Buffer.AddFront(data[i]);
				break;
			case SeekMode::End:
			case SeekMode::Advance:
				m_Buffer.AddRange(data.cbegin(), data.cend());
				break;
		}
	}

	void File::ClearBuffer()
	{
		m_Buffer.Clear();
	}

	std::string File::GetLine(const char delim)
	{
		std::string line{};
		while (m_BufferPointer < m_Buffer.Size())
		{
			line.push_back(m_Buffer[m_BufferPointer++]);

			if (m_Buffer[m_BufferPointer] == delim)
			{
				line.push_back(m_Buffer[m_BufferPointer++]);
				break;
			}
		}

		return line;
	}

	void File::Seek(const SeekMode mode, const uint64_t val)
	{
		__ASSERT(!m_Buffer.Empty());

		switch (mode)
		{
			case SeekMode::Start:
				m_BufferPointer = val;
				break;
			case SeekMode::End:
				m_BufferPointer = m_Buffer.Size() - 1 - val;
				break;
			case SeekMode::Advance:
				m_BufferPointer += val;

				if (m_BufferPointer >= m_Buffer.Size())
					m_BufferPointer = m_Buffer.Size() - 1u;
				break;
			case SeekMode::Regress:
				if (val <= m_BufferPointer)
					m_BufferPointer -= val;
				else
					m_BufferPointer = 0;
				break;
		}
	}

	const TArray<char>& File::GetBuffer() const
	{
		return m_Buffer;
	}
}