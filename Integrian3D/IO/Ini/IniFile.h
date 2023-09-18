#pragma once

#include "EngineConstants.h"
#include "Array/Array.h"
#include "DebugUtility/DebugUtility.h"
#include "IO/File/File.h"
#include "Math/Math.h"
#include "TPair/TPair.full.h"

#include <string>
#include <unordered_map>

namespace Integrian3D
{
	namespace Detail
	{
		template<typename T>
		class IniDataMap final
		{
		public:
			__NODISCARD T& operator[](const std::string& Key)
			{
				const auto It{ DataIndices.find(Key) };
				const int32 DataIndex{ It != DataIndices.end() ? It->second : AddEmptyKeyValue(Key) };
				
				return Data[DataIndex].Value();
			}

			__NODISCARD bool ContainsKey(const std::string& Key) const
			{
				return DataIndices.contains(Key);
			}

			__NODISCARD const T& Get(const std::string& Key) const
			{
				__ASSERT(ContainsKey(Key), "Missing Key in Ini Map: %s", Key.c_str());

				return Data[DataIndices.find(Key)->second].Value();
			}

			int32 AddEmptyKeyValue(const std::string& Key)
			{
				const int32 Index{ static_cast<int32>(Data.Size()) };
				DataIndices[Key] = Index;
				Data.EmplaceBack(Key, T{});

				return Index;
			}

		private:
			std::unordered_map<std::string, int32> DataIndices;
			TArray<TPair<std::string, T>> Data;
		};
	}

	class IniFile final
	{
	public:
		explicit IniFile(const std::string_view Filepath);

		bool GetString(const std::string& Section, const std::string& Key, std::string& Value) const;
		bool GetInteger(const std::string& Section, const std::string& Key, int32& Value) const;
		bool GetFloat(const std::string& Section, const std::string& Key, float& Value) const;
		bool GetBoolean(const std::string& Section, const std::string& Key, bool& bValue) const;
		bool GetVector2D(const std::string& Section, const std::string& Key, Math::Vec2D& Value) const;
		bool GetVector3D(const std::string& Section, const std::string& Key, Math::Vec3D& Value) const;
		// [TODO]: Add array support

	private:
		void ParseIniFile(const std::string_view FileContents);

		IO::File File;
		Detail::IniDataMap<Detail::IniDataMap<std::string>> KeyValuePairs;
	};
}