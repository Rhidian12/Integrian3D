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
		, Filepointer{}
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
		, Filepointer{ __MOVE(Other.Filepointer) }
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
		Filepointer = __MOVE(Other.Filepointer);

		Other.Handle = nullptr;

		return *this;
	}

	void File::Seek(const int64 NewFilepointerPos)
	{
		Filepointer = NewFilepointerPos;
	}

	void File::Advance(const int64 AdvanceAmount)
	{
		Filepointer += AdvanceAmount;

		if (Filepointer > Filesize)
		{
			Filepointer = Filesize;
		}
	}

	void File::Regress(const int64 RegressAmount)
	{
		Filepointer -= RegressAmount;

		if (Filepointer < 0)
		{
			Filepointer = 0;
		}
	}

	const std::string_view File::GetFilepath() const
	{
		return Filepath;
	}

	std::string File::GetFileContents() const
	{
		__CHECK(Handle != nullptr && Handle != INVALID_HANDLE_VALUE);

		std::string FileContents{};
		FileContents.resize(Filesize);

		if (ReadFile(Handle, FileContents.data(), static_cast<DWORD>(Filesize), nullptr, nullptr) == 0)
		{
			LOG(File, Warning, "File could not read the provided file: %s", Filepath);
		}

		return FileContents;
	}

	int64 File::GetFilesize() const
	{
		return Filesize;
	}

	int64 File::GetFilepointer() const
	{
		return Filepointer;
	}

	File& File::operator<<(const char* String)
	{
		__CHECK(Handle != nullptr && Handle != INVALID_HANDLE_VALUE);

		const int32 Size{ static_cast<int32>(strlen(String)) };
		if (Mode == FileMode::ASCII)
		{
			if (WriteFile(Handle, String, Size, nullptr, nullptr) == 0)
			{
				LOG(File, Warning, "File::operator<< could not write to file %s", Filepath);
			}
			else
			{
				Filesize += Size;
			}
		}
		else
		{
			static constexpr int32 BufferSize{ 32 };
			char Buffer[BufferSize]{};

			__ASSERT(Size <= (BufferSize - sizeof(uint8)), "File::operator<< Strings cannot be larger than %i characters when writing Binary files", BufferSize - 1);

			const uint8 CastSize{ static_cast<uint8>(Size) };
			Buffer[0] = *reinterpret_cast<const char*>(&CastSize);

			for (int32 i{}; i < Size; ++i)
			{
				Buffer[i + 1] = String[i];
			}

			if (WriteFile(Handle, Buffer, Size + 1, nullptr, nullptr) == 0)
			{
				LOG(File, Warning, "File::operator<< could not write to file %s", Filepath);
			}
			else
			{
				Filesize += Size + 1;
			}
		}

		return *this;
	}

	File& File::operator<<(const std::string& String)
	{
		__CHECK(Handle != nullptr && Handle != INVALID_HANDLE_VALUE);

		const int32 Size{ static_cast<int32>(String.size()) };

		if (Mode == FileMode::ASCII)
		{
			if (WriteFile(Handle, String.data(), static_cast<DWORD>(String.size()), nullptr, nullptr) == 0)
			{
				LOG(File, Warning, "File::operator<< could not write to file %s", Filepath);
			}
			else
			{
				Filesize += String.size();
			}
		}
		else
		{
			static constexpr int32 BufferSize{ 32 };
			char Buffer[BufferSize]{};

			__ASSERT(Size <= (BufferSize - sizeof(uint8)), "File::operator<< Strings cannot be larger than %i characters when writing Binary files", BufferSize - 1);

			const uint8 CastSize{ static_cast<uint8>(Size) };
			Buffer[0] = *reinterpret_cast<const char*>(&CastSize);

			for (int32 i{ 1 }; i < Size; ++i)
			{
				Buffer[i] = String[i - 1];
			}

			if (WriteFile(Handle, Buffer, Size, nullptr, nullptr) == 0)
			{
				LOG(File, Warning, "File::operator<< could not write to file %s", Filepath);
			}
			else
			{
				Filesize += Size + 1;
			}
		}

		return *this;
	}

	File& File::operator>>(std::string& String)
	{
		__CHECK(Handle != nullptr && Handle != INVALID_HANDLE_VALUE);
		__CHECK(SetFilePointer(Handle, static_cast<LONG>(Filepointer), nullptr, FILE_BEGIN) != INVALID_SET_FILE_POINTER);

		if (Mode == FileMode::ASCII)
		{
			bool bContinue{ true };
			while (bContinue)
			{
				char CurrentChar{};
				if (ReadFile(Handle, &CurrentChar, 1, nullptr, nullptr) == 0)
				{
					LOG(File, Warning, "File::operator>> could not read from file: %s", Filepath);
					break;
				}

				const bool bIsNewline{ CurrentChar == '\n' };
				bContinue = Filepointer < Filesize && !bIsNewline;

				if (bContinue)
				{
					String += CurrentChar;
				}
				else if (bIsNewline)
				{

				}
			}
		}
		else
		{
			uint8 StringSize{};
			*this >> StringSize;

			String.resize(StringSize);

			if (ReadFile(Handle, String.data(), static_cast<DWORD>(StringSize), nullptr, nullptr) == 0)
			{
				LOG(File, Warning, "File::operator>> could not read from file: %s", Filepath);
			}
			else
			{
				Filepointer += StringSize;
			}
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