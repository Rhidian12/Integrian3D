#include "File.h"

#include "IO/PathUtils.h"
#include "IO/FileContentCache/FileContentCache.h"
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
	File::File(const std::string_view Filepath, const OpenMode OpenMode, const FileMode Mode, const bool bMonitorFile)
		: Filepath{ Filepath }
		, Handle{ INVALID_HANDLE_VALUE }
		, Filesize{}
		, Mode{ Mode }
	{
		Handle = OpenFile(OpenMode);

		if (!Handle.IsValid())
		{
			LOG(FileLog, LogErrorLevel::Error, "File could not open the provided file: %s Error:", Filepath);
			return;
		}

		CalculateFilesize();

		FileContentCache::GetInstance().AddFile(this, bMonitorFile);
	}

	File::~File()
	{
		FileContentCache::GetInstance().RemoveFile(this);
	}

	File::File(File&& Other) noexcept
		: Filepath{ I_MOVE(Other.Filepath) }
		, Handle{ I_MOVE(Other.Handle) }
		, Filesize{ I_MOVE(Other.Filesize) }
		, Mode{ I_MOVE(Other.Mode) }
	{}

	File& File::operator=(File&& Other) noexcept
	{
		Filepath = I_MOVE(Other.Filepath);
		Handle = I_MOVE(Other.Handle);
		Filesize = I_MOVE(Other.Filesize);
		Mode = I_MOVE(Other.Mode);

		return *this;
	}

	void File::Seek(const int32 NewFilepointerPos) const
	{
		CALL_WIN32(SetFilePointer(Handle, NewFilepointerPos, nullptr, FILE_BEGIN));
	}

	void File::Advance(const int32 AdvanceAmount) const
	{
		CALL_WIN32(SetFilePointer(Handle, AdvanceAmount, nullptr, FILE_CURRENT));
	}

	void File::Regress(const int32 RegressAmount) const
	{
		CALL_WIN32(SetFilePointer(Handle, RegressAmount, nullptr, FILE_CURRENT));
	}

	const std::string_view File::GetFilepath() const
	{
		return Filepath;
	}

	std::string File::GetFileContents() const
	{
		return std::string{ FileContentCache::GetInstance().GetFileContents(Filepath) };
	}

	int32 File::GetFilesize() const
	{
		return Filesize;
	}

	int32 File::GetFilepointer() const
	{
		return static_cast<int32>(CALL_WIN32_RV(SetFilePointer(Handle, 0, nullptr, FILE_CURRENT)));
	}

	void File::StartMonitoringFile()
	{
		FileContentCache::GetInstance().StartMonitoringFileForChanges(this);
	}

	void File::StopMonitoringFile()
	{
		FileContentCache::GetInstance().StopMonitoringFileForChanges(this);
	}

	void File::BindToOnFileChanged(const std::function<void(std::string)>& Callback)
	{
		FileContentCache::GetInstance().BindToOnFileChanged(Callback);
	}

	File& File::operator<<(const char* String)
	{
		__CHECK(Handle.IsValid());

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
		__CHECK(Handle.IsValid());

		*this << String.c_str();

		return *this;
	}

	File& File::operator<<(const bool bValue)
	{
		__CHECK(Handle.IsValid());

		*this << (bValue ? static_cast<int8>(1) : static_cast<int8>(0));

		return *this;
	}

	const File& File::operator>>(std::string& String) const
	{
		__CHECK(Handle.IsValid());

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
		__CHECK(Handle.IsValid());

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
			LOG(FileLog, LogErrorLevel::Warning, "File::operator>> could not read from file {}", Filepath);
		}

		return CurrentChar;
	}

	void File::ReadFromFile(char* Buffer, const int32 BufferSize) const
	{
		auto Call = CALL_WIN32(ReadFile(Handle, Buffer, static_cast<DWORD>(BufferSize), nullptr, nullptr));

		if (!Call.GetSuccess())
		{
			LOG(FileLog, LogErrorLevel::Warning, "File::operator>> could not read from file {}", Filepath);
		}
	}

	void File::GetFileContents_Implementation(std::string& OutFileContents)
	{
		__CHECK(Handle.IsValid());

		OutFileContents.resize(Filesize);

		if (CALL_WIN32_RV(ReadFile(Handle, OutFileContents.data(), static_cast<DWORD>(Filesize), nullptr, nullptr)) == 0)
		{
			LOG(FileLog, LogErrorLevel::Warning, "File could not read the provided file: {}", Filepath);
		}

		Seek(0);
	}

	void File::CalculateFilesize()
	{
		Filesize = static_cast<int32>(CALL_WIN32_RV(GetFileSize(Handle, nullptr)));
	}

	void File::WriteToFile(const char* Buffer, const int32 BufferSize)
	{
		if (CALL_WIN32_RV(WriteFile(Handle, Buffer, static_cast<DWORD>(BufferSize), nullptr, nullptr)) == 0)
		{
			LOG(FileLog, LogErrorLevel::Warning, "File::operator<< could not write to file {}", Filepath);
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
			ICHECK_MSG(!PathUtils::DoesFileExist(Filepath), "File::File() > OpenMode::CreateNew > File {} already exists", Filepath);
			ErrorToIgnore = ERROR_FILE_EXISTS;
			break;
		case OpenMode::OpenExisting:
			ICHECK_MSG(PathUtils::DoesFileExist(Filepath), "File::File() > OpenMode::OpenExisting > File {} does not exist", Filepath);
			ErrorToIgnore = ERROR_FILE_NOT_FOUND;
			break;
		case OpenMode::TruncateExisting:
			ICHECK_MSG(PathUtils::DoesFileExist(Filepath), "File::File() > OpenMode::TruncateExisting > File {} does not exist", Filepath);
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
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					nullptr,
					static_cast<DWORD>(OpenMode),
					FILE_ATTRIBUTE_NORMAL,
					nullptr
				),
				ErrorToIgnore
			);
	}
}