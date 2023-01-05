#pragma once

#include "../../EngineConstants.h"
#include "../File/File.h"
#include "../Serializer/Serializer.h"

#include <string> /* std::string */

namespace Integrian3D::IO
{
	/// <summary>
	/// An RAII .iasset file writer	that can write .iasset files 
	/// </summary>
	class IAssetWriter final
	{
	public:
		explicit IAssetWriter(std::string filepath);
		
		IAssetWriter(const IAssetWriter&) noexcept = delete;
		IAssetWriter(IAssetWriter&& other) noexcept = default;
		IAssetWriter& operator=(const IAssetWriter&) noexcept = delete;
		IAssetWriter& operator=(IAssetWriter&& other) noexcept = default;

		template<typename T>
		void Serialize(const T& val)
		{
			/*
			All values little-endian
			+00 6B Magic Identification (currently IASSET)
			+06 1B Version Number (currently 1)
			+07 1B Padding
			+08 4B Length of File
			+0C 2B Offset to data (counted from start of file)
			+XX [Data]
			*/

			/* Write magic id */
			const std::string iasset{ "IASSET" };
			constexpr uint8_t version{ 1u };
			constexpr uint8_t padding{ '\0' };

			const TArray<char> serializedData{ ::Serialize(val) };
			m_File.Write(serializedData);

			const uint32_t length{ static_cast<uint32_t>(serializedData.Size()) };
			const uint16_t offset{ static_cast<uint16_t>(iasset.size() + sizeof(version) + sizeof(padding) +
				sizeof(length) + sizeof(offset)) };

			m_File.Write(::Serialize(offset), SeekMode::Start);
			m_File.Write(::Serialize(length), SeekMode::Start);

			m_File.Write(::Serialize(padding), SeekMode::Start);
			m_File.Write(::Serialize(version), SeekMode::Start);

			for (int i{ static_cast<int>(iasset.size() - 1) }; i >= 0; --i)
				m_File.Write(::Serialize(iasset[i]), SeekMode::Start);

			m_File.Write();
		}

	private:
		File m_File;
	};
}