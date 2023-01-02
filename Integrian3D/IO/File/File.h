#pragma once

#include "../../EngineConstants.h"
#include "../../Array/Array.h"
#include "../SeekMode.h"

#include <string> /* std::string */

namespace Integrian3D::IO
{
	/// <summary>
	/// An RAII File abstraction. Loads in the requested file into memory upon creation.
	/// Writing and Reading from a file leads to Undefined Behaviour, as File uses an internal pointer to its
	/// internal buffer, writing to the buffer might offset the pointer.
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

		/// <summary>
		/// Write all data in the internal buffer to the file.
		/// </summary>
		void Write() const;

		/// <summary>
		/// Write data to the internal buffer.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="data">: Data to write to the internal buffer.</param>
		/// <param name="mode">: Where to add the data in the buffer.</param>
		template<typename T>
		void Write(T&& data, const SeekMode mode = SeekMode::Advance)
		{
			__ASSERT(mode != SeekMode::Regress && "Cannot write while regressing");

			switch (mode)
			{
			case SeekMode::Start:
				for (int32_t i{ static_cast<int32_t>(amountOfData) - 1 }; i >= 0; --i)
					m_Buffer.AddFront(*(reinterpret_cast<const char*>(&data) + i));
				break;
			case SeekMode::End:
			case SeekMode::Advance:
				/* Add size of string */
				Write(amountOfData, mode);

				/* Add serialized string */
				for (uint64_t i{}; i < amountOfData; ++i)
					m_Buffer.Add(*(reinterpret_cast<const char*>(&data) + i));
				break;
			}
		}

		/// <summary>
		/// Reads data from the internal buffer, according to the size of T
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns>a std::string which can be deserialized by a user-defined function or an engine provided one</returns>
		template<typename T>
		std::string Read()
		{
			__ASSERT(!m_Buffer.Empty());

			std::string data{};
			data.resize(sizeof(T));

			for (uint64_t i{}; i < data; ++i)
				data[i] = m_Buffer[m_BufferPointer++];

			return data;
		}

		/// <summary>
		/// Set the internal buffer pointer, this does NOT affect Write(), only Read()
		/// </summary>
		/// <param name="mode">: From where to set the buffer pointer</param>
		/// <param name="val">: From the mode selected, what offset to apply to the buffer pointer</param>
		void Seek(const SeekMode mode, const uint64_t val);

		__NODISCARD const TArray<char>& GetBuffer() const;

	private:
		std::string m_Filepath;
		void* m_pHandle;
		TArray<char> m_Buffer;
		uint64_t m_BufferPointer;
	};
}