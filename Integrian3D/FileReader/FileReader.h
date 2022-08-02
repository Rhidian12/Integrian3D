#pragma once

#include <string> /* std::string */
#include <vector> /* std::vector */ /* [CRINGE]: Use my own implementation */

namespace Integrian3D
{
	class FileReader final
	{
	public:
		explicit FileReader(const std::string& filePath);
		~FileReader();

		const std::string& GetFilePath() const { return FilePath; }
		const std::string& GetFileContents() const { return FileContents; }

	private:
		void* Handle; /* HANDLE */
		std::string FilePath;
		std::string FileContents;
	};
}