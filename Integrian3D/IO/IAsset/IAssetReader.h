#pragma once

#include "../../EngineConstants.h"
#include "../File/File.h"
#include "../Serializer/Serializer.h"

#include <string> /* std::string */

namespace Integrian3D::IO
{
	/// <summary>
	/// An RAII .iasset file reader	that can read .iasset files 
	/// </summary>
	class IAssetReader final
	{
	public:
		explicit IAssetReader(std::string filepath);

		IAssetReader(const IAssetReader&) noexcept = delete;
		IAssetReader(IAssetReader&& other) noexcept = default;
		IAssetReader& operator=(const IAssetReader&) noexcept = delete;
		IAssetReader& operator=(IAssetReader&& other) noexcept = default;

		template<typename T>
		void Deserialize(T& val)
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

			//std::string iasset{};
			//iasset.resize(6);
			//uint8_t version{};
			//uint32_t length{};
			//uint16_t offset{};

			//iasset = ::Deserialize<std::string>(m_File.Read<std::string>());

			//__ASSERT(iasset == "IASSET");

			//version = ::Deserialize<uint8_t>(m_File.Read<uint8_t>());

			///* advance past padding */
			//m_File.Seek(SeekMode::Advance, 1u);

			//length = ::Deserialize<uint32_t>(m_File.Read<uint32_t>());
			//offset = ::Deserialize<uint16_t>(m_File.Read<uint16_t>());

			//m_File.Seek(SeekMode::Start, offset);

			//::Deserialize<T>(m_File.Read<T>(), val);
		}

		template<typename T>
		T Deserialize()
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

			//std::string iasset{};
			//iasset.resize(6);
			//uint8_t version{};
			//uint32_t length{};
			//uint16_t offset{};

			//for (size_t i{}; i < iasset.size(); ++i)
			//	iasset[i] = ::Deserialize<char>(m_File.Read<char>());

			//__ASSERT(iasset == "IASSET");

			//version = ::Deserialize<uint8_t>(m_File.Read<uint8_t>());

			///* advance past padding */
			//m_File.Seek(SeekMode::Advance, 1u);

			//length = ::Deserialize<uint32_t>(m_File.Read<uint32_t>());
			//offset = ::Deserialize<uint16_t>(m_File.Read<uint16_t>());

			//m_File.Seek(SeekMode::Start, offset);

			//return ::Deserialize<T>(m_File.Read<T>());

			return T{};
		}

	private:
		// File m_File;
	};
}