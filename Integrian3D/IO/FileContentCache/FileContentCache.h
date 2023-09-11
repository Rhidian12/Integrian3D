#pragma once

#include "EngineConstants.h"

#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <unordered_map>

DECLARE_LOG_CATEGORY(FileContentCache)

namespace Integrian3D::IO
{
	class FileContentCache final
	{
	private:
		struct FileInfo final
		{
			std::string Filepath;
			int32 ReferenceCounter;
		};

		struct FileInfoHasher final
		{
			SIZE_T operator()(const FileInfo& FInfo) const
			{
				return std::hash<std::string>{}(FInfo.Filepath);
			}
		};

		struct FileInfoComparer final
		{
			bool operator()(const FileInfo& A, const FileInfo& B) const
			{
				return A.Filepath == B.Filepath;
			}
		};

	public:
		__NODISCARD static FileContentCache& GetInstance();

		void AddFile(class File* const File);
		void RemoveFile(const class File* const File);

		__NODISCARD std::string_view GetFileContents(const std::string_view Filepath) const;

	private:
		FileContentCache() = default;
		__NODISCARD bool ContainsFilepath(const std::string_view Filepath, FileInfo& OutFileInfo) const;

		friend std::unique_ptr<FileContentCache> std::make_unique();
		inline static std::unique_ptr<FileContentCache> Instance{};

		std::mutex Mutex;

		// [TODO]: Create custom Unordered Map
		std::unordered_map<FileInfo, std::string, FileInfoHasher, FileInfoComparer> FileContentsCache; // [File | FileContents]
	};
}