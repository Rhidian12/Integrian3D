#pragma once

#include "../../EngineConstants.h"
#include "../../Array/Array.h"
#include "SeekMode.h"

#include <string_view> /* std::string_view */

namespace Integrian3D::IO
{
	class BinaryWriter final
	{
	public:
		explicit BinaryWriter(const std::string_view filepath);
		~BinaryWriter();

		template<typename T>
		void WriteData(const T& data, const SeekMode mode = SeekMode::Advance)
		{
			__ASSERT(mode != SeekMode::Regress && "Cannot write while regressing");

			char splitData[sizeof(T)]{};

			for (uint64_t i{}; i < sizeof(T); ++i)
				splitData[i] = *(reinterpret_cast<char*>(&data) + i);

			switch (mode)
			{
			case SeekMode::Start:
				for (int i{ sizeof(T) - 1 }; i >= 0; --i)
					m_Buffer.AddFront(splitData[i]);
				break;
			case SeekMode::End:
			case SeekMode::Advance:
				m_Buffer.AddRange(splitData, sizeof(T));
				break;
			}
		}

		void WriteToFile() const;

	private:
		void* m_pHandle;
		TArray<char> m_Buffer;
	};
}