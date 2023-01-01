#pragma once

#include "../../EngineConstants.h"
#include "../../Array/Array.h"
#include "SeekMode.h"
#include "BinaryUtils.h"

#include <string_view> /* std::string_view */
#include <string> /* std::string */

namespace Integrian3D::IO
{
	/// <summary>
	/// An RAII Binary File Reader that reads in the requested file when it is constructed
	/// Use ReadData to access the buffer and cast the data from it to a specific type
	/// </summary>
	class BinaryReader final
	{
	public:
		BinaryReader();
		explicit BinaryReader(const std::string_view filepath);
		~BinaryReader();

		BinaryReader(const BinaryReader&) noexcept = delete;
		BinaryReader(BinaryReader&& other) noexcept;
		BinaryReader& operator=(const BinaryReader&) noexcept = delete;
		BinaryReader& operator=(BinaryReader&& other) noexcept;

		template<typename T>
		__NODISCARD T Read()
		{
			__ASSERT(!m_Buffer.Empty());

			using RawT = std::remove_cvref_t<T>;

			static_assert(!std::is_pointer_v<RawT>,
				"BinaryReader::Read<T>() > T cannot be a pointer");
			static_assert(HasDeserialize<RawT> || IsPod<RawT>,
				"BinaryReader::Read<T>() > T must be POD or provide a void Serialize(std::string) method");
			static_assert(std::is_default_constructible_v<RawT>,
				"BinaryReader::Read<T>() > T must be default constructible, if this is not possible, use BinaryReader::Read<T>(T& data)");

			if constexpr (HasDeserialize<RawT>)
			{
				/* First read the uint64_t to get the size */
				const uint64_t size{ Read<uint64_t>() };
				std::string serializedData{};
				serializedData.resize(size);

				for (uint32_t i{}; i < size; ++i)
					serializedData[i] = m_Buffer[m_BufferPointer++];

				T data{};
				data.Deserialize(serializedData);

				return data;
			}
			else /* IsPod<T> */
			{
				const RawT data{ SwapEndianness<RawT>(*reinterpret_cast<RawT*>(&m_Buffer[m_BufferPointer])) };

				m_BufferPointer += sizeof(RawT);

				return data;
			}
		}
		template<typename T>
		void Read(T& data)
		{
			__ASSERT(!m_Buffer.Empty());

			using RawT = std::remove_cvref_t<T>;

			static_assert(!std::is_pointer_v<RawT>,
				"BinaryReader::Read<T>() > T cannot be a pointer");
			static_assert(HasDeserialize<RawT> || IsPod<RawT>,
				"BinaryReader::Read<T>() > T must be POD or provide a void Serialize(std::string) method");
			static_assert(std::is_default_constructible_v<RawT>,
				"BinaryReader::Read<T>() > T must be default constructible, if this is not possible, use BinaryReader::Read<T>(T& data)");

			if constexpr (HasDeserialize<RawT>)
			{
				/* First read the uint64_t to get the size */
				const uint64_t size{ Read<uint64_t>() };
				std::string serializedData{};
				serializedData.resize(size);

				for (uint32_t i{}; i < size; ++i)
					serializedData[i] = m_Buffer[m_BufferPointer++];

				data.Deserialize(serializedData);
			}
			else /* IsPod<T> */
			{
				data = SwapEndianness<RawT>(*reinterpret_cast<RawT*>(&m_Buffer[m_BufferPointer]));

				m_BufferPointer += sizeof(RawT);
			}
		}

		void Seek(const SeekMode mode, const uint64_t val)
		{
			__ASSERT(!m_Buffer.Empty());

			switch (mode)
			{
			case SeekMode::Start:
				m_BufferPointer = val;
				break;
			case SeekMode::End:
				m_BufferPointer = m_Buffer.Size() - 1 - val;
				break;
			case SeekMode::Advance:
				m_BufferPointer += val;

				if (m_BufferPointer >= m_Buffer.Size())
					m_BufferPointer = m_Buffer.Size() - 1u;
				break;
			case SeekMode::Regress:
				if (val <= m_BufferPointer)
					m_BufferPointer -= val;
				else
					m_BufferPointer = 0;
				break;
			}
		}

		__NODISCARD const TArray<char>& GetFileContents() const
		{
			return m_Buffer;
		}

	private:
		void* m_pHandle;
		TArray<char> m_Buffer;
		uint64_t m_BufferPointer;
	};
}