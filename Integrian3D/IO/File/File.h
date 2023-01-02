#pragma once

#include "../../EngineConstants.h"
#include "../../Array/Array.h"

#include <string> /* std::string */

namespace Integrian3D::IO
{
	/// <summary>
	/// An RAII File abstraction. Loads in the requested file into memory upon creation.
	/// </summary>
	class File final
	{
	public:
		File();
		explicit File(const std::string& filepath);
		~File();

		File(const File&) noexcept = delete;
		File(File&& other) noexcept;
		File& operator=(const File&) noexcept = delete;
		File& operator=(File&& other) noexcept;

		void Write() const;

		__NODISCARD TArray<char>& GetBuffer();
		__NODISCARD const TArray<char>& GetBuffer() const;

	private:
		std::string m_Filepath;
		void* m_pHandle;
		TArray<char> m_Buffer;
	};
}