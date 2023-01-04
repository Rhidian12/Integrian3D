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
	template<typename T>
	class IAssetWriter final
	{
	public:
		explicit IAssetWriter(std::string filepath)
		{
			filepath = filepath.substr(0, filepath.find_last_of('.'));
			filepath.append(".iasset");

			m_File = File{ filepath };
		}

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

			const TArray<char> serializedData{ Serialize(val) };

			const uint32_t length{ static_cast<uint32_t>(serializedData.GetBuffer().Size()) };
			const uint16_t offset{ static_cast<uint16_t>(iasset.size() + sizeof(version) + sizeof(padding) +
				sizeof(length) + sizeof(offset)) };

			m_File.Write(Serialize(offset), SeekMode::Start);
			m_File.Write(Serialize(length), SeekMode::Start);

			m_File.Write(Serialize(padding), SeekMode::Start);
			m_File.Write(Serialize(version), SeekMode::Start);
			
			m_File.Write(Serialize(iasset), SeekMode::Start);

			m_File.Write();
		}

	private:
		File m_File;
	};
}