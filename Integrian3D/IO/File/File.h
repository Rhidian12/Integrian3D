#pragma once

#include "../../EngineConstants.h"
#include "../../Array/Array.h"
#include "../SeekMode.h"
#include "../Serializer/Serializer.h"

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

		void Write(const TArray<char>& data, const SeekMode mode = SeekMode::Advance)
		{
			__ASSERT(mode != SeekMode::Regress && "Cannot write while regressing");

			switch (mode)
			{
			case SeekMode::Start:
				for (int32_t i{ static_cast<int32_t>(data.Size()) - 1 }; i >= 0; --i)
					m_Buffer.AddFront(data[i]);
				break;
			case SeekMode::End:
			case SeekMode::Advance:
				m_Buffer.AddRange(data.cbegin(), data.cend());
				break;
			}
		}

		/// <summary>
		/// Reads data from the internal buffer, according to the size of T
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns>a std::string which can be deserialized by a user-defined function or an engine provided one</returns>
		template<typename T>
		TArray<char> Read()
		{
			__ASSERT(!m_Buffer.Empty());

			TArray<char> data{};
			data.Resize(sizeof(T));

			for (uint64_t i{}; i < data.Size(); ++i)
				data[i] = m_Buffer[m_BufferPointer++];

			return data;
		}

		template<>
		TArray<char> Read<std::string>()
		{
			__ASSERT(!m_Buffer.Empty());

			TArray<char> data{};
			const uint32_t size{ Deserialize<uint32_t>(m_Buffer) };
			m_BufferPointer += sizeof(size);

			for (uint64_t i{}; i < size; ++i)
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