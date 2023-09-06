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
//	/// An RAII .iasset file reader	that can read .iasset files 
//	/// </summary>
//	class IAssetReader final
//	{
//	public:
//		template<typename T>
//		static void Deserialize(const std::string_view Filepath, T& Val)
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
//			const File File{ Filepath, OpenMode::OpenExisting, FileMode::Binary };
//
//			static constexpr int8 Offset{ 8 };
//			// skip to the offset
//			File.Seek(Offset);
//
//			int16 OffsetToData{};
//			// File >> OffsetToData;
//
//			File.Seek(OffsetToData);
//
//			// File >> Val;
//		}
//	};
//}