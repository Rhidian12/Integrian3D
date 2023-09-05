#pragma once

#include "../../EngineConstants.h"
#include "../File/File.h"
#include "../Serializer/Serializer.h"

#include <string_view> /* std::string */

namespace Integrian3D::IO
{
	/// <summary>
	/// An RAII .iasset file writer	that can write .iasset files 
	/// </summary>
	class IAssetWriter final
	{
	public:
		IAssetWriter(const IAssetWriter&) noexcept = delete;
		IAssetWriter(IAssetWriter&&) noexcept = delete;
		IAssetWriter& operator=(const IAssetWriter&) noexcept = delete;
		IAssetWriter& operator=(IAssetWriter&&) noexcept = delete;

		template<typename T>
		static void Serialize(const std::string_view Filepath, const T& Val)
		{
			/*
				All values little-endian
				+XX [Memory Address] | X Bytes [Size of Header] | Description

				+00 | 6 Bytes | Magic Identification (currently IASSET)
				+06 | 1 Bytes | Version Number (currently 1)
				+07 | 1 Bytes | Padding
				+08 | 2 Bytes | Offset to data (counted from start of file)
				+XX | X Bytes | Data
			*/

			File File{ Filepath, OpenMode::CreateAlways, FileMode::Binary };
					
			/* Write magic id */
			const static std::string iasset{ "IASSET" };
			constexpr int8 version{ 1 }; // [TODO]: Read this in from a config file
			constexpr int8 padding{ '\0' };

			File << iasset << version << padding;

			const int16 offset{ static_cast<int16>(iasset.size() + sizeof(version) + sizeof(padding) + sizeof(offset)) };

			File << offset << Val;
		}
	};
}