#include "IO/FileContentCache/FileContentCache.h"

#include "IO/File/File.h"
#include "TPair/TPair.full.h"

#include <algorithm>

namespace Integrian3D::IO
{
	FileContentCache& FileContentCache::GetInstance()
	{
		static FileContentCache Instance{};

		return Instance;
	}

	FileContentCache::FileContentCache()
	{
		Monitor.BindToOnFileChanged(std::bind(&FileContentCache::OnFileChanged, this, std::placeholders::_1));
	}

	FileContentCache::~FileContentCache()
	{
		LOG(FileContentCacheLog, LogErrorLevel::Log, "Destroying file content cache, remaining entries {}", FileContentsCache.Size());

		if (!FileContentsCache.Empty())
		{
			for (int32 i{}; i < FileContentsCache.Size(); ++i)
			{
				LOG(FileContentCacheLog, LogErrorLevel::Log, "Remaining file #{}: {}", i, FileContentsCache[i].Key().Filepath);
			}
		}
	}

	void FileContentCache::AddFile(File* const File, const bool bShouldMonitorFile)
	{
		const std::string_view Filepath{ File->GetFilepath() };

		if (ContainsFilepath(Filepath))
		{
			FileInfo& FileInfo{ GetFileInfo(Filepath) };

			const std::unique_lock<std::mutex> Lock{ Mutex };

			++FileInfo.ReferenceCounter;
			return;
		}

		const std::unique_lock<std::mutex> Lock{ Mutex };

		if (bShouldMonitorFile)
		{
			StartMonitoringFileForChanges(File);
		}

		std::string FileContents{};
		File->GetFileContents_Implementation(FileContents);
		FileContentsCache.Add(MakePair(FileContentCache::FileInfo{ Filepath.data(), File, 1 }, FileContents));
	}

	void FileContentCache::RemoveFile(const File* const File)
	{
		const std::string_view Filepath{ File->GetFilepath() };
		if (!ContainsFilepath(Filepath))
		{
			LOG(FileContentCacheLog, LogErrorLevel::Warning, "FileContentCache is trying to remove {} which isn't present!", Filepath);
			return;
		}

		{
			StopMonitoringFileForChanges(File);

			FileInfo& FileInfo{ GetFileInfo(Filepath) };

			const std::unique_lock<std::mutex> Lock{ Mutex };

			if (--FileInfo.ReferenceCounter <= 0)
			{
				FileContentsCache.Erase([&FileInfo](const TPair<FileContentCache::FileInfo, std::string>& Pair)->bool
					{
						return FileInfo == Pair.Key();
					});
			}
		}
	}

	void FileContentCache::StartMonitoringFileForChanges(const File* const File)
	{
		Monitor.StartMonitoringFile(File->GetFilepath().data());
	}

	void FileContentCache::StopMonitoringFileForChanges(const File* const File)
	{
		Monitor.StopMonitoringFile(File->GetFilepath().data());
	}

	void FileContentCache::BindToOnFileChanged(const std::function<void(std::string)>& Callback)
	{
		Monitor.BindToOnFileChanged(Callback);
	}

	std::string_view FileContentCache::GetFileContents(const std::string_view Filepath) const
	{
		if (ContainsFilepath(Filepath))
		{
			const std::unique_lock<std::mutex> Lock{ Mutex };

			return FileContentsCache.Find([Filepath](const TPair<FileInfo, std::string>& Pair)->bool
				{
					return Filepath == Pair.Key().Filepath;
				})->Value();
		}

		return "";
	}

	bool FileContentCache::ContainsFilepath(const std::string_view Filepath) const
	{
		const std::unique_lock<std::mutex> Lock{ Mutex };

		const auto CIt{ FileContentsCache.Find([Filepath](const TPair<FileInfo, std::string>& Pair)->bool
			{
				return Pair.Key().Filepath == Filepath;
			})};

		return CIt != FileContentsCache.cend();
	}

	void FileContentCache::OnFileChanged(const std::string& Filepath)
	{
		if (ContainsFilepath(Filepath))
		{
			FileInfo& FileInfo{ GetFileInfo(Filepath) };

			std::string FileContents{};
			FileInfo.File->CalculateFilesize();
			FileInfo.File->GetFileContents_Implementation(FileContents);
			FileContentsCache.Find([&FileInfo](const auto& Pair)->bool
				{
					return FileInfo.Filepath == Pair.Key().Filepath;
				})->Value() = FileContents;
		}
	}

	FileContentCache::FileInfo& FileContentCache::GetFileInfo(const std::string_view Filepath)
	{
		IASSERT(ContainsFilepath(Filepath));

		const std::unique_lock<std::mutex> Lock{ Mutex };

		return FileContentsCache.Find([Filepath](const TPair<FileInfo, std::string>& Pair)->bool
			{
				return Filepath == Pair.Key().Filepath;
			})->Key();
	}
}