#include "File.h"

#include "../../DebugUtility/DebugUtility.h"

#include <string>

namespace Integrian3D::IO
{
	namespace
	{
		static bool DoesFileExist(const std::string_view Filepath)
		{
			DWORD Attributes = GetFileAttributesA(Filepath.data());

			return Attributes != INVALID_FILE_ATTRIBUTES && !(Attributes & FILE_ATTRIBUTE_DIRECTORY);
		}
	}

	File::File(const std::string_view Filepath, const OpenMode OpenMode, const FileMode Mode)
		: Filepath{ Filepath }
		, Handle{}
		, Filesize{}
		, Mode{ Mode }
	{
		Handle = OpenFile(OpenMode);

		if (Handle == INVALID_HANDLE_VALUE || !Handle)
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

	std::string File::GetFileContents() const
	{
		std::string FileContents{};
		FileContents.resize(Filesize);

		if (ReadFile(Handle, FileContents.data(), static_cast<DWORD>(Filesize), nullptr, nullptr) == 0)
		{
			LOG(File, Warning, "File could not read the provided file: %s", Filepath);
		}

		return FileContents;
	}

	File& File::operator<<(const char* String)
	{
		const int32_t Size{ static_cast<int32_t>(strlen(String)) };

		if (WriteFile(Handle, String, Size, nullptr, nullptr) == 0)
		{
			LOG(File, Warning, "File::operator<< could not write to file %s", Filepath);
		}
		else
		{
			Filesize += Size;
		}

		return *this;
	}

	File& File::operator<<(const std::string& String)
	{
		if (WriteFile(Handle, String.data(), static_cast<DWORD>(String.size()), nullptr, nullptr) == 0)
		{
			LOG(File, Warning, "File::operator<< could not write to file %s", Filepath);
		}
		else
		{
			Filesize += String.size();
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

	void* File::OpenFile(const OpenMode OpenMode) const
	{
		switch (OpenMode)
		{
			case OpenMode::CreateNew:
				__ASSERT(!DoesFileExist(Filepath), "File::File() > OpenMode::CreateNew > File %s already exists", Filepath);
				break;
			case OpenMode::OpenExisting:
				__ASSERT(DoesFileExist(Filepath), "File::File() > OpenMode::OpenExisting > File %s already exists", Filepath);
				break;
			case OpenMode::TruncateExisting:
				__ASSERT(DoesFileExist(Filepath), "File::File() > OpenMode::TruncateExisting > File %s does not exist", Filepath);
				break;
			default:
				break;
		}

		return CreateFileA(Filepath.data(),
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ,
			nullptr,
			static_cast<DWORD>(OpenMode),
			FILE_ATTRIBUTE_NORMAL,
			nullptr);
	}
}