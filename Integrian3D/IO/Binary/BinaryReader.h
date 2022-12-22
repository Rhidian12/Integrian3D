#pragma once

#include "../../EngineConstants.h"
#include "../../Array/Array.h"
#include "SeekMode.h"

#include <string_view> /* std::string_view */

namespace Integrian3D::IO
{
	/// <summary>
	/// An RAII Binary File Reader that reads in the requested file when it is constructed
	/// Use ReadData to access the buffer and cast the data from it to a specific type
	/// </summary>
	class BinaryReader final
	{
	public:
		explicit BinaryReader(const std::string_view filepath);
		~BinaryReader();

		template<typename T>
		__NODISCARD T ReadData(bool advanceBuffer = true)
		{
			if (m_Buffer.Empty())
				return;

			/* [TODO]: Fix this */
			static_assert(std::is_pod_v<T>, "BinaryReader::ReadData<T>() -> T is not a POD value, no support implemented yet");

			const T data{ *reinterpret_cast<T*>(&m_Buffer[m_BufferPointer]) };

			if (advanceBuffer)
				m_BufferPointer += sizeof(T);
			
			return data;
		}
		template<typename T>
		void ReadData(T& data, bool advanceBuffer = true)
		{
			if (m_Buffer.Empty())
				return;

			/* [TODO]: Fix this */
			static_assert(std::is_pod_v<T>, "BinaryReader::ReadData<T>() -> T is not a POD value, no support implemented yet");

			data = *reinterpret_cast<T*>(&m_Buffer[m_BufferPointer]);

			if (advanceBuffer)
				m_BufferPointer += sizeof(T);
		}

		void Seek(const SeekMode mode, const uint64_t val)
		{
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