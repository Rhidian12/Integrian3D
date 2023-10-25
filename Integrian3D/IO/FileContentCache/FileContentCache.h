#pragma once

#include "EngineConstants.h"

#include "Array/Array.h"
#include "TPair/TPair.h"

#include <memory>
#include <mutex>
#include <string>
#include <string_view>

DEFINE_LOG_CATEGORY(FileContentCacheLog, Integrian3D::LogVerbosity::Verbose)

namespace Integrian3D::IO
{
	class File;

	class FileContentCache final
	{
	private:
		struct FileInfo final
		{
			std::string Filepath;
			File* File;
			int32 ReferenceCounter;

			bool operator==(const FileInfo& Other) const
			{
				return Filepath == Other.Filepath;
			}
		};

	public:
		__NODISCARD static FileContentCache& GetInstance();

		void AddFile(File* const File);
		void RemoveFile(const File* const File);

		__NODISCARD std::string_view GetFileContents(const std::string_view Filepath) const;

	private:
		FileContentCache() = default;
		__NODISCARD bool ContainsFilepath(const std::string_view Filepath) const;
		void OnFileChanged(const std::string& Filepath);
		__NODISCARD FileInfo& GetFileInfo(const std::string_view Filepath);

		friend std::unique_ptr<FileContentCache> std::make_unique();
		inline static std::unique_ptr<FileContentCache> Instance{};

		mutable std::mutex Mutex;

		// [TODO]: Create custom Unordered Map
		TArray<TPair<FileInfo, std::string>> FileContentsCache; // [File | FileContents]
	};
}