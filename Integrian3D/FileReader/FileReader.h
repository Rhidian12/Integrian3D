#pragma once

#include "../EngineConstants.h"

#include <string> /* std::string */
#include <vector> /* std::vector */

namespace Integrian3D
{
	/* RAII File reader */
	class FileReader final
	{
	public:
		explicit FileReader(const std::string& filePath, const char delimiter = '\n');
		~FileReader();

		FileReader(const FileReader&) noexcept = delete;
		FileReader(FileReader&& other) noexcept;
		FileReader& operator=(const FileReader&) noexcept = delete;
		FileReader& operator=(FileReader&& other) noexcept;

		__NODISCARD const std::string& GetFilePath() const { return FilePath; }
		__NODISCARD const std::string& GetFileContents() const { return FileContents; }
		__NODISCARD const std::vector<std::string>& GetDelimitedFileContents() const { return DelimitedFileContents; }

		__NODISCARD const std::string& GetLine(const size_t lineNumber) const { __ASSERT(lineNumber < DelimitedFileContents.size()); return DelimitedFileContents[lineNumber]; }

	private:
		void* Handle; /* HANDLE */
		std::string FilePath;
		std::vector<std::string> DelimitedFileContents;
		std::string FileContents;
	};
}