#include "BinaryWriter.h"

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
	BinaryWriter::BinaryWriter()
		: m_pHandle{}
		, m_Buffer{}
	{}

	BinaryWriter::BinaryWriter(const std::string_view filepath)
		: m_pHandle{}
		, m_Buffer{}
	{
		/* open the file */
		m_pHandle = CreateFileA(filepath.data(),
			GENERIC_WRITE,
			FILE_SHARE_WRITE,
			nullptr,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			nullptr);

		__ASSERT(m_pHandle != INVALID_HANDLE_VALUE && "BinaryWriter could not create a file at the provided path");
	}

	BinaryWriter::~BinaryWriter()
	{
		if (m_pHandle)	
			if (CloseHandle(m_pHandle) == 0)
				Debug::LogError("BinaryWriter could not close the provided file", false);
	}

	BinaryWriter::BinaryWriter(BinaryWriter&& other) noexcept
		: m_pHandle{ __MOVE(other.m_pHandle) }
		, m_Buffer{ __MOVE(other.m_Buffer) }
	{
		other.m_pHandle = nullptr;
	}

	BinaryWriter& BinaryWriter::operator=(BinaryWriter&& other) noexcept
	{
		m_pHandle = __MOVE(other.m_pHandle);
		m_Buffer = __MOVE(other.m_Buffer);

		other.m_pHandle = nullptr;

		return *this;
	}

	void BinaryWriter::WriteToFile() const
	{
		if (WriteFile(m_pHandle, m_Buffer.Data(), static_cast<DWORD>(m_Buffer.Size()), nullptr, nullptr) == 0)
			Debug::LogError("BinaryWriter could not write the data to the provided file", false);
	}
}