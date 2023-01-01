#pragma once

#include "../../EngineConstants.h"
#include "../Binary/BinaryReader.h"
#include "IAssetUtils.h"

#include <string> /* std::string */

namespace Integrian3D::IO
{
	/// <summary>
	/// An RAII .iasset file reader	that can read .iasset files 
	/// </summary>
	template<typename Conv, typename T>
	class IAssetReader final
	{
	public:
		explicit IAssetReader(std::string filepath)
			: m_Converter{}
		{
			filepath = filepath.substr(0, filepath.find_last_of('.'));
			filepath.append(".iasset");

			m_Reader = BinaryReader{ filepath.c_str() };
		}

		void Deserialize(T& val)
		{
			static_assert(IAsset_HasDeserialize<Conv, T>, "Converter::Deserialize(BinaryReader&, T&) > Not correctly implemented");

			/*
			All values little-endian
			+00 6B Magic Identification (currently IASSET)
			+06 1B Version Number (currently 1)
			+07 1B Padding
			+08 4B Length of File
			+0C 2B Offset to data (counted from start of file)
			+XX [Data]
			*/

			std::string iasset{};
			iasset.resize(6);
			uint8_t version{};
			uint32_t length{};
			uint16_t offset{};

			for (size_t i{}; i < iasset.size(); ++i)
				iasset[i] = m_Reader.Read<char>();

			__ASSERT(iasset == "IASSET");

			version = m_Reader.Read<uint8_t>();

			/* advance past padding */
			m_Reader.Seek(SeekMode::Advance, 1u);

			length = m_Reader.Read<uint32_t>();
			offset = m_Reader.Read<uint16_t>();

			m_Reader.Seek(SeekMode::Start, offset);

			m_Converter.Deserialize(m_Reader, val);
		}

		T Deserialize()
		{
			static_assert(IAsset_HasDeserializeRet<Conv, T>, "Converter::Deserialize(BinaryReader&, T&) > Not correctly implemented");
			static_assert(std::is_default_constructible_v<T>, "IAssetReader::Deserialize() > T must be default constructible");

			/*
			All values little-endian
			+00 6B Magic Identification (currently IASSET)
			+06 1B Version Number (currently 1)
			+07 1B Padding
			+08 4B Length of File
			+0C 2B Offset to data (counted from start of file)
			+XX [Data]
			*/

			std::string iasset{};
			iasset.resize(6);
			uint8_t version{};
			uint32_t length{};
			uint16_t offset{};

			for (size_t i{}; i < iasset.size(); ++i)
				iasset[i] = m_Reader.Read<char>();

			__ASSERT(iasset == "IASSET");

			version = m_Reader.Read<uint8_t>();

			/* advance past padding */
			m_Reader.Seek(SeekMode::Advance, 1u);

			length = m_Reader.Read<uint32_t>();
			offset = m_Reader.Read<uint16_t>();

			m_Reader.Seek(SeekMode::Start, offset);

			return m_Converter.Deserialize(m_Reader);
		}


	private:
		Conv m_Converter;
		BinaryReader m_Reader;
	};
}