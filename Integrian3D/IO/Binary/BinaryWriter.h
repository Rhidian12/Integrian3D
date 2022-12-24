#pragma once

#include "../../EngineConstants.h"
#include "../../Array/Array.h"
#include "SeekMode.h"
#include "BinaryUtils.h"

#include <string_view> /* std::string_view */
#include <string> /* std::string */

namespace Integrian3D::IO
{
	class BinaryWriter final
	{
	public:
		explicit BinaryWriter(const std::string_view filepath);
		~BinaryWriter();

		template<typename T>
		void Write(T&& data, const SeekMode mode = SeekMode::Advance)
		{
			__ASSERT(mode != SeekMode::Regress && "Cannot write while regressing");

			using RawT = std::remove_reference_t<T>;

			static_assert(!std::is_pointer_v<RawT>,
				"BinaryWriter::Write<T>() > T cannot be a pointer");
			static_assert(HasSerialize<RawT> || IsPod<RawT>,
				"BinaryWriter::Write<T>() > T must be POD or provide a std::string Serialize() method");

			if constexpr (HasSerialize<RawT>)
			{
				const std::string serializedData{ data.Serialize() };
				uint64_t amountOfData{ serializedData.size() };

				switch (mode)
				{
				case SeekMode::Start:
					/* Add serialized string */
					for (int32_t i{ static_cast<int32_t>(amountOfData) - 1 }; i >= 0; --i)
						m_Buffer.AddFront(serializedData[i]);
					
					/* Add size of string */
					Write(amountOfData, mode);
					break;
				case SeekMode::End:
				case SeekMode::Advance:
					/* Add size of string */
					Write(amountOfData, mode);

					/* Add serialized string */
					for (uint64_t i{}; i < amountOfData; ++i)
						m_Buffer.Add(serializedData[i]);
					break;
				}
			}
			else /* IsPod<RawT> */
			{
				RawT convertedData{ SwapEndianness<RawT>(data) };

				char splitData[sizeof(RawT)]{};

				for (uint64_t i{}; i < sizeof(RawT); ++i)
					splitData[i] = *(reinterpret_cast<char*>(&convertedData) + i);

				switch (mode)
				{
				case SeekMode::Start:
					for (int i{ sizeof(RawT) - 1 }; i >= 0; --i)
						m_Buffer.AddFront(splitData[i]);
					break;
				case SeekMode::End:
				case SeekMode::Advance:
					m_Buffer.AddRange(splitData, sizeof(RawT));
					break;
				}
			}
		}

		void WriteToFile() const;

	private:
		void* m_pHandle;
		TArray<char> m_Buffer;
	};
}