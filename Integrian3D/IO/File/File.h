#pragma once

#include "../../EngineConstants.h"
#include "../../Array/Array.h"
#include "../../Delegate/Delegate.h"
#include "../SeekMode.h"
#include "../FileMode.h"
#include "../OpenMode.h"
#include "../Serializer/Serializer.h"

#include <string_view>

namespace Integrian3D::IO
{
	/// <summary>
	/// Minimal RAII file abstraction.
	/// </summary>
	class File final
	{
	public:
		explicit File(const std::string_view Filepath, const OpenMode OpenMode, const FileMode Mode);
		~File();

		File(const File&) noexcept = delete;
		File(File&& Other) noexcept;
		File& operator=(const File&) noexcept = delete;
		File& operator=(File&& Other) noexcept;

		const std::string_view GetFilepath() const;
		const int64_t GetFilesize() const;
		TArray<unsigned char> GetFileContents() const;

		template<typename T>
		File& operator<<(const T& Val);

	private:
		void CloseHandle();

		std::string_view Filepath;
		void* Handle;
		int64_t Filesize;
		FileMode Mode;
	};

	template<typename T>
	inline File& File::operator<<(const T& Val)
	{
		__ASSERT(Mode == FileMode::ASCII, "File::operator<< is only available on ASCII files");
		constexpr static int32_t Size{ sizeof(Val) };

		unsigned char Buffer[Size]{};
		memcpy(Buffer, &Val, Size);

		if (WriteFile(Handle, Buffer, Size, nullptr, nullptr) == 0)
		{
			LOG(File, Warning, "File::operator<< could not write to file %s", Filepath);
		}

		return *this;
	}
}