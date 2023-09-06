//#pragma once
//
//#include "../../EngineConstants.h"
//#include "../File/File.h"
//
//#include <string_view>
//
//namespace Integrian3D::IO
//{
//	/// <summary>
//	/// An RAII .iasset file writer	that can write .iasset files 
//	/// </summary>
//	class IAssetWriter final
//	{
//	public:
//		IAssetWriter(const IAssetWriter&) noexcept = delete;
//		IAssetWriter(IAssetWriter&&) noexcept = delete;
//		IAssetWriter& operator=(const IAssetWriter&) noexcept = delete;
//		IAssetWriter& operator=(IAssetWriter&&) noexcept = delete;
//
//		template<typename T>
//		static void Serialize(const std::string_view Filepath, const T& Val)
//		{
//			/*
//				All values little-endian
//				+XX [Memory Address] | X Bytes [Size of Header] | Description
//
//				+00 | 6 Bytes | Magic Identification (currently IASSET)
//				+06 | 1 Bytes | Version Number (currently 1)
//				+07 | 1 Bytes | Padding
//				+08 | 2 Bytes | Offset to data (counted from start of file)
//				+XX | X Bytes | Data
//			*/
//
//			File File{ Filepath, OpenMode::CreateAlways, FileMode::Binary };
//					
//			const static std::string IASSET{ "IASSET" };
//			constexpr int8 Version{ 1 }; // [TODO]: Read this in from a config file
//			constexpr int8 Padding{ '\0' };
//
//			// File << IASSET << Version << Padding;
//
//			const int16 Offset{ static_cast<int16>(IASSET.size() + sizeof(Version) + sizeof(Padding) + sizeof(Offset)) };
//
//			// File << Offset << Val;
//		}
//	};
//}