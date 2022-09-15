#pragma once

#include "../EngineConstants.h"

#include <string> /* std::string */
#include <vector> /* std::vector */ /* [CRINGE]: Use my own implementation */

namespace Integrian3D
{
	class FileReader final
	{
	public:
		explicit FileReader(const std::string& filePath);
		~FileReader();

		FileReader(const FileReader&) noexcept = delete;
		FileReader(FileReader&& other) noexcept;
		FileReader& operator=(const FileReader&) noexcept = delete;
		FileReader& operator=(FileReader&& other) noexcept;

		__NODISCARD const std::string& GetFilePath() const { return FilePath; }
		__NODISCARD const std::string& GetFileContents() const { return FileContents; }

	private:
		void* Handle; /* HANDLE */
		std::string FilePath;
		std::string FileContents;
	};
}