#include "File.h"
#include "Win32Utils/Win32APICall.h"

#ifdef _WIN32
#	pragma warning ( push )
#	pragma warning ( disable : 4005 ) /* warning C4005: 'APIENTRY': macro redefinition */ 
#		define WIN32_LEAN_AND_MEAN
#		include <Windows.h>
#	pragma warning ( pop )
#endif

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
		, Handle{ INVALID_HANDLE_VALUE }
		, Filesize{}
		, Mode{ Mode }
	{
		Handle = OpenFile(OpenMode);

		if (!Handle.IsValid())
		{
			LOG(File, Error, "File could not open the provided file: %s Error:", Filepath);
			return;
		}

		Filesize = static_cast<int32>(GetFileSize(Handle, nullptr));
	}

	File::File(File&& Other) noexcept
		: Filepath{ __MOVE(Other.Filepath) }
		, Handle{ __MOVE(Other.Handle) }
		, Filesize{ __MOVE(Other.Filesize) }
		, Mode{ __MOVE(Other.Mode) }
	{}

	File& File::operator=(File&& Other) noexcept
	{
		Filepath = __MOVE(Other.Filepath);
		Handle = __MOVE(Other.Handle);
		Filesize = __MOVE(Other.Filesize);
		Mode = __MOVE(Other.Mode);

		return *this;
	}

	void File::Seek(const int32 NewFilepointerPos) const
	{
		SetFilePointer(Handle, NewFilepointerPos, nullptr, FILE_BEGIN);
	}

	void File::Advance(const int32 AdvanceAmount) const
	{
		SetFilePointer(Handle, AdvanceAmount, nullptr, FILE_CURRENT);
	}

	void File::Regress(const int32 RegressAmount) const
	{
		SetFilePointer(Handle, RegressAmount, nullptr, FILE_CURRENT);
	}

	const std::string_view File::GetFilepath() const
	{
		return Filepath;
	}

	std::string File::GetFileContents() const
	{
		__CHECK(IsHandleValid());

		std::string FileContents{};
		FileContents.resize(Filesize);

		if (ReadFile(Handle, FileContents.data(), static_cast<DWORD>(Filesize), nullptr, nullptr) == 0)
		{
			LOG(File, Warning, "File could not read the provided file: %s", Filepath);
		}

		Seek(0);

		return FileContents;
	}

	int32 File::GetFilesize() const
	{
		return Filesize;
	}

	int32 File::GetFilepointer() const
	{
		return static_cast<int32>(SetFilePointer(Handle, 0, nullptr, FILE_CURRENT));
	}

	File& File::operator<<(const char* String)
	{
		__CHECK(IsHandleValid());

		const int32 Size{ static_cast<int32>(strlen(String)) };
		if (Mode == FileMode::Binary)
		{
			*this << Size;
		}
		WriteToFile(String, Size);

		return *this;
	}

	File& File::operator<<(const std::string& String)
	{
		__CHECK(IsHandleValid());

		*this << String.c_str();

		return *this;
	}

	File& File::operator<<(const bool bValue)
	{
		__CHECK(IsHandleValid());

		*this << (bValue ? static_cast<int8>(1) : static_cast<int8>(0));

		return *this;
	}

	const File& File::operator>>(std::string& String) const
	{
		__CHECK(IsHandleValid());

		if (Mode == FileMode::ASCII)
		{
			bool bContinue{ true };
			while (bContinue)
			{
				char CurrentChar{ ReadCharacterFromFile() };

				const bool bIsNewline{ CurrentChar == '\n' };
				const bool bIsEOF{ CurrentChar == '\0' };
				bContinue = !(bIsNewline || bIsEOF);

				if (bContinue)
				{
					String += CurrentChar;
				}
			}
		}
		else
		{
			int32 StringSize{};
			*this >> StringSize;

			String.resize(StringSize);

			ReadFromFile(String.data(), StringSize);
		}

		return *this;
	}

	const File& File::operator>>(bool& bValue) const
	{
		__CHECK(IsHandleValid());

		int8 Value{};
		*this >> Value;

		bValue = static_cast<bool>(Value);

		return *this;
	}

	char File::ReadCharacterFromFile() const
	{
		char CurrentChar{ '\0' };
		auto Call = CALL_WIN32(ReadFile(Handle, &CurrentChar, 1, nullptr, nullptr));

		if (!Call.GetSuccess())
		{
			LOG(File, Warning, "File::operator<< could not write to file %s", Filepath);
		}

		return CurrentChar;
	}

	void File::ReadFromFile(char* Buffer, const int32 BufferSize) const
	{
		auto Call = CALL_WIN32(ReadFile(Handle, Buffer, static_cast<DWORD>(BufferSize), nullptr, nullptr));

		if (!Call.GetSuccess())
		{
			LOG(File, Warning, "File::operator<< could not write to file %s", Filepath);
		}
	}

	void File::WriteToFile(const char* Buffer, const int32 BufferSize)
	{
		auto Call = CALL_WIN32(WriteFile(Handle, Buffer, static_cast<DWORD>(BufferSize), nullptr, nullptr));

		if (!Call.GetSuccess())
		{
			LOG(File, Warning, "File::operator<< could not write to file %s", Filepath);
		}
		else
		{
			Filesize += BufferSize;
		}
	}

	void* File::OpenFile(const OpenMode OpenMode) const
	{
		DWORD ErrorToIgnore{};

		switch (OpenMode)
		{
		case OpenMode::CreateNew:
			__ASSERT(!DoesFileExist(Filepath), "File::File() > OpenMode::CreateNew > File %s already exists", Filepath);
			ErrorToIgnore = ERROR_FILE_EXISTS;
			break;
		case OpenMode::OpenExisting:
			__ASSERT(DoesFileExist(Filepath), "File::File() > OpenMode::OpenExisting > File %s already exists", Filepath);
			ErrorToIgnore = ERROR_FILE_NOT_FOUND;
			break;
		case OpenMode::TruncateExisting:
			__ASSERT(DoesFileExist(Filepath), "File::File() > OpenMode::TruncateExisting > File %s does not exist", Filepath);
			ErrorToIgnore = ERROR_FILE_NOT_FOUND;
			break;
		case OpenMode::CreateAlways:
		case OpenMode::OpenAlways:
			ErrorToIgnore = ERROR_ALREADY_EXISTS;
			break;
		default:
			break;
		}

		return CALL_WIN32_RV_IGNORE_ERROR
			(
				CreateFileA
				(
					Filepath.data(),
					GENERIC_READ | GENERIC_WRITE,
					FILE_SHARE_READ,
					nullptr,
					static_cast<DWORD>(OpenMode),
					FILE_ATTRIBUTE_NORMAL,
					nullptr
				),
				ErrorToIgnore
			);
	}

	bool File::IsHandleValid() const
	{
		return Handle != INVALID_HANDLE_VALUE;
	}
}