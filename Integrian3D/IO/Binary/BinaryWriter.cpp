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
	BinaryWriter::BinaryWriter(const std::string_view filepath)
		: m_pHandle{}
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
		if (CloseHandle(m_pHandle) == 0)
			Debug::LogError("BinaryWriter could not close the provided file", false);
	}

	void BinaryWriter::WriteToFile() const
	{
		if (WriteFile(m_pHandle, m_Buffer.Data(), m_Buffer.Size(), nullptr, nullptr) == 0)
			Debug::LogError("BinaryWriter could not write the data to the provided file", false);
	}
}