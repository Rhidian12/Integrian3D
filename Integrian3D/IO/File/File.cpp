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
	File::File(const std::string_view Filepath, const OpenMode OpenMode, const FileMode Mode)
		: Filepath{ Filepath }
		, Handle{}
		, Filesize{}
		, Mode{ Mode }
	{
		/* open the file */
		Handle = CreateFileA(Filepath.data(),
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ,
			nullptr,
			static_cast<DWORD>(OpenMode),
			FILE_ATTRIBUTE_NORMAL,
			nullptr);

		if (Handle == INVALID_HANDLE_VALUE)
		{
			LOG(File, Error, "File could not open the provided file: %s", Filepath);
			return;
		}

		/* Get the file size */
		Filesize = static_cast<int64_t>(GetFileSize(Handle, nullptr));
	}

	File::~File()
	{
		CloseHandle();
	}

	File::File(File&& other) noexcept
		: Filepath{ __MOVE(other.Filepath) }
		, Handle{ __MOVE(other.Handle) }
		, Filesize{ __MOVE(other.Filesize) }
	{
		other.Handle = nullptr;
	}

	File& File::operator=(File&& other) noexcept
	{
		if (Handle)
		{
			CloseHandle();
		}

		Filepath = __MOVE(other.Filepath);
		Handle = __MOVE(other.Handle);
		Filesize = __MOVE(other.Filesize);

		other.Handle = nullptr;

		return *this;
	}

	const std::string_view File::GetFilepath() const
	{
		return Filepath;
	}

	const int64_t File::GetFilesize() const
	{
		return Filesize;
	}

	TArray<unsigned char> File::GetFileContents() const
	{
		TArray<unsigned char> FileContents{};

		FileContents.Resize(Filesize);

		/* Read the file contents */
		DWORD readBytes{};
		if (ReadFile(Handle, FileContents.Data(), static_cast<DWORD>(Filesize), &readBytes, nullptr) == 0)
		{
			LOG(File, Error, "File could not read the provided file: %s", Filepath);
		}

		return FileContents;
	}

	void File::CloseHandle()
	{
		if (Handle)
		{
			if (::CloseHandle(Handle) == 0)
			{
				LOG(File, Error, "File could not close the provided file: %s", Filepath);
			}

			Handle = nullptr;
		}
	}
}