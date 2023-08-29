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
		explicit File(const std::string_view Filepath, const OpenMode OpenMode);
		~File();

		File(const File&) noexcept = delete;
		File(File&& other) noexcept;
		File& operator=(const File&) noexcept = delete;
		File& operator=(File&& other) noexcept;

		const std::string_view GetFilepath() const;
		const int64_t GetFilesize() const;

		void OnFileChanged();
		Delegate<File*>& GetOnFileChangedDelegate();

	private:
		void CloseHandle();

		Delegate<File*> OnFileChanged;
		std::string_view Filepath;
		void* Handle;
		int64_t Filesize;
	};
}