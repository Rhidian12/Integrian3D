#include "IO/FileContentCache/FileContentCache.h"

#include "IO/File/File.h"

#include <algorithm>

namespace Integrian3D::IO
{
	FileContentCache& FileContentCache::GetInstance()
	{
		if (!Instance)
		{
			Instance = std::make_unique<FileContentCache>();
		}

		return *Instance;
	}

	void FileContentCache::AddFile(File* const File)
	{
		std::scoped_lock<std::mutex> Lock{ Mutex };
		const std::string_view Filepath{ File->GetFilepath() };

		FileInfo FileInfo{};
		if (ContainsFilepath(Filepath, FileInfo))
		{
			++FileInfo.ReferenceCounter;
			return;
		}

		std::string FileContents{};
		File->GetFileContents_Implementation(FileContents);
		FileContentsCache.insert(std::make_pair(FileContentCache::FileInfo{ Filepath.data(), 1 }, FileContents));
	}

	void FileContentCache::RemoveFile(const File* const File)
	{
		const std::string_view Filepath{ File->GetFilepath() };
		FileInfo FileInfo{};

		if (!ContainsFilepath(Filepath, FileInfo))
		{
			LOG(FileContentCache, Fatal, "FileContentCache is trying to remove %s which was never added!", Filepath.data());
			return;
		}

		{
			std::scoped_lock<std::mutex> Lock{ Mutex };

			if (--FileInfo.ReferenceCounter <= 0)
			{
				FileContentsCache.erase(FileInfo);
			}
		}
	}

	std::string_view FileContentCache::GetFileContents(const std::string_view Filepath) const
	{
		FileInfo FileInfo{};

		if (ContainsFilepath(Filepath, FileInfo))
		{
			return FileContentsCache.at(FileInfo);
		}

		return "";
	}

	bool FileContentCache::ContainsFilepath(const std::string_view Filepath, FileInfo& OutFileInfo) const
	{
		const auto CIt{ std::find_if(FileContentsCache.cbegin(), FileContentsCache.cend(), [Filepath](const auto& FileInfoPair)->bool
			{
				return FileInfoPair.first.Filepath == Filepath;
			}) };

		if (CIt != FileContentsCache.cend())
		{
			OutFileInfo = CIt->first;
			return true;
		}

		return false;
	}
}