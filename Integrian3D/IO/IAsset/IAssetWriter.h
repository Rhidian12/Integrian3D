#pragma once

#include "../../EngineConstants.h"
#include "../Binary/BinaryWriter.h"
#include "IAssetUtils.h"

#include <string> /* std::string */

namespace Integrian3D::IO
{
	/// <summary>
	/// An RAII .iasset file writer	that can write .iasset files 
	/// </summary>
	template<typename Conv, typename T>
	class IAssetWriter final
	{
	public:
		explicit IAssetWriter(std::string filepath)
			: m_Converter{}
		{
			filepath = filepath.substr(0, filepath.find_last_of('.'));
			filepath.append(".iasset");

			m_Writer = BinaryWriter{ filepath.c_str() };
		}

		void Serialize(const T& val)
		{
			static_assert(IAsset_HasSerialize<Conv, T>, "Converter::Serialize(BinaryWriter&, T) > Not correctly implemented");

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

			m_Converter.Serialize(m_Writer, val);

			const uint32_t length{ static_cast<uint32_t>(m_Writer.GetBuffer().Size()) };
			const uint16_t offset{ static_cast<uint16_t>(iasset.size() + sizeof(version) + sizeof(padding) +
				sizeof(length) + sizeof(offset)) };

			m_Writer.Write(offset, SeekMode::Start);
			m_Writer.Write(length, SeekMode::Start);

			m_Writer.Write(padding, SeekMode::Start);
			m_Writer.Write(version, SeekMode::Start);
			
			for (int i{ static_cast<int>(iasset.size() - 1) }; i >= 0; --i)
				m_Writer.Write(iasset[i], SeekMode::Start);

			m_Writer.WriteToFile();
		}

	private:
		Conv m_Converter;
		BinaryWriter m_Writer;
	};
}