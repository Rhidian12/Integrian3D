#include "IAssetWriter.h"

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
	IAssetWriter::IAssetWriter(std::string filepath)
		: m_pHandle{}
	{
		filepath = filepath.substr(0, filepath.find_last_of('.'));
		filepath.append(".iasset");

		/* open the file */
		m_pHandle = CreateFileA(filepath.data(),
			GENERIC_WRITE,
			FILE_SHARE_WRITE,
			nullptr,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			nullptr);

		__ASSERT(m_pHandle != INVALID_HANDLE_VALUE && "IAssetWriter could not create a file at the provided path");
	}

	IAssetWriter::~IAssetWriter()
	{
		if (CloseHandle(m_pHandle) == 0)
			Debug::LogError("IAssetWriter could not close the provided file", false);
	}
}