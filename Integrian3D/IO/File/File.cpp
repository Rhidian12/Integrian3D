#include "File.h"

#include "../../DebugUtility/DebugUtility.h"

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

	File::File(File&& Other) noexcept
		: Filepath{ __MOVE(Other.Filepath) }
		, Handle{ __MOVE(Other.Handle) }
		, Filesize{ __MOVE(Other.Filesize) }
		, Mode{ __MOVE(Other.Mode) }
	{
		Other.Handle = nullptr;
	}

	File& File::operator=(File&& Other) noexcept
	{
		if (Handle)
		{
			CloseHandle();
		}

		Filepath = __MOVE(Other.Filepath);
		Handle = __MOVE(Other.Handle);
		Filesize = __MOVE(Other.Filesize);
		Mode = __MOVE(Other.Mode);

		Other.Handle = nullptr;

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

	TArray<char> File::GetFileContents() const
	{
		TArray<char> FileContents{};

		FileContents.Resize(Filesize);

		/* Read the file contents */
		DWORD readBytes{};
		if (ReadFile(Handle, FileContents.Data(), static_cast<DWORD>(Filesize), &readBytes, nullptr) == 0)
		{
			LOG(File, Error, "File could not read the provided file: %s", Filepath);
		}

		return FileContents;
	}

	File& File::operator<<(const TArray<char>& SerializedData)
	{
		for (const char c : SerializedData)
		{
			*this << c;
		}
		
		return *this;
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