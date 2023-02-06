#pragma once

#include "../../EngineConstants.h"
#include "../../Array/Array.h"
#include "../SeekMode.h"
#include "../FileMode.h"
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

		void Write(const TArray<char>& data, const SeekMode mode = SeekMode::Advance);

		/// <summary>
		/// Reads data from the internal buffer, according to the size of T
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns>a std::string which can be deserialized by a user-defined function or an engine provided one</returns>
		template<typename T>
		__NODISCARD TArray<char> Read()
		{
			__ASSERT(!m_Buffer.Empty());

			TArray<char> data{};
			data.Resize(sizeof(T));

			for (uint64_t i{}; i < data.Size(); ++i)
				data[i] = m_Buffer[m_BufferPointer++];

			return data;
		}

		template<>
		__NODISCARD TArray<char> Read<std::string>()
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

		void ClearBuffer();

		__NODISCARD std::string GetLine(const char delim = '\n');

		__NODISCARD const TArray<char>& GetBuffer() const;

	#pragma region Operator<<

		File & operator<<(const bool val)
		{
			val ? m_Buffer.Add('1') : m_Buffer.Add('0');
		
			return *this;
		}

		File& operator<<(const char val)
		{
			m_Buffer.Add(val);

			return *this;
		}

		File& operator<<(const int8_t val)
		{
			InsertIntegralNumber(val);

			return *this;
		}

		File& operator<<(const uint8_t val)
		{
			InsertIntegralNumber(val);

			return *this;
		}

		File& operator<<(const int16_t val)
		{
			InsertIntegralNumber(val);

			return *this;
		}

		File& operator<<(const uint16_t val)
		{
			InsertIntegralNumber(val);

			return *this;
		}

		File& operator<<(const int32_t val)
		{
			InsertIntegralNumber(val);

			return *this;
		}

		File& operator<<(const uint32_t val)
		{
			InsertIntegralNumber(val);

			return *this;
		}

		File& operator<<(const int64_t val)
		{
			InsertIntegralNumber(val);

			return *this;
		}

		File& operator<<(const uint64_t val)
		{
			InsertIntegralNumber(val);

			return *this;
		}

		File& operator<<(const float val)
		{
			const int len{ _scprintf("%f", val) };
			std::string str(len, '\0');
			sprintf_s(&str[0], len + 1, "%f", val);

			for (const char c : str)
				m_Buffer.Add(c);

			return *this;
		}

		File& operator<<(const double val)
		{
			const int len{ _scprintf("%f", val) };
			std::string str(len, '\0');
			sprintf_s(&str[0], len + 1, "%f", val);

			for (const char c : str)
				m_Buffer.Add(c);

			return *this;
		}

		File& operator<<(const std::string& val)
		{
			for (const char c : val)
				m_Buffer.Add(c);

			return *this;
		}

		File& operator<<(const char* pStr)
		{
			while (*pStr != '\0')
				m_Buffer.Add(*pStr++);

			return *this;
		}

	#pragma endregion

	private:
		template<typename T>
		void InsertIntegralNumber(const T val)
		{
			static_assert(std::is_integral_v<T>, "File::InsertIntegralNumber(T) > T must be integral");
		
			if (val >= 10)
				InsertIntegralNumber(val / 10);

			m_Buffer.Add(static_cast<char>(val) + '0');
		}

		std::string m_Filepath;
		void* m_pHandle;
		TArray<char> m_Buffer;
		uint64_t m_BufferPointer;
	};
}