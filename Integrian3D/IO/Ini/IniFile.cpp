#include "IO/Ini/IniFile.h"
#include "Math/Math.h"

#include <algorithm>
#include <sstream>

namespace Integrian3D
{
	namespace
	{
		__INLINE static bool IsLineComment(const std::string_view Line)
		{
			return Line[0] == ';';
		}

		__INLINE static bool IsLineSection(const std::string_view Line)
		{
			return Line[0] == '[';
		}

		__INLINE static void StringToLowercase(std::string& String)
		{
			std::transform(String.begin(), String.end(), String.begin(), [](const char Char)->char
				{
					return static_cast<char>(std::tolower(static_cast<unsigned char>(Char)));
				}); 
		}

		__INLINE static void TrimWhiteSpace(std::string& String)
		{
			String.erase(std::remove_if(String.begin(), String.end(), std::isspace), String.end());
		}

		static void SplitString(const std::string& OriginalString, const char SplitCharacter, TArray<std::string>& SplitParts)
		{
			SIZE_T Start{};
			SIZE_T SplitCharacterPos{ OriginalString.find(SplitCharacter) };

			while (SplitCharacterPos != std::string::npos)
			{
				SplitParts.Add(OriginalString.substr(Start, SplitCharacterPos - Start));

				Start = SplitCharacterPos + 1;
				SplitCharacterPos = OriginalString.find(SplitCharacter, Start);
			}

			if (Start < OriginalString.size())
			{
				const std::string FinalPart{ OriginalString.substr(Start) };

				bool bIsWhitespace{ true };
				for (const char Character : FinalPart)
				{
					if (!std::isspace(Character))
					{
						bIsWhitespace = false;
						break;
					}
				}

				if (!bIsWhitespace)
				{
					SplitParts.Add(OriginalString.substr(Start));
				}
			}
		}
	}

	IniFile::IniFile(const std::string_view Filepath)
		: File{ Filepath, IO::OpenMode::OpenExisting, IO::FileMode::ASCII }
	{
		ParseIniFile(File.GetFileContents());
	}

	bool IniFile::GetString(const std::string& Section, const std::string& Key, std::string& Value) const
	{
		if (KeyValuePairs.ContainsKey(Section))
		{
			const auto& IniSection{ KeyValuePairs.Get(Section) };
			if (IniSection.ContainsKey(Key))
			{
				Value = IniSection.Get(Key);
				return true;
			}
		}

		return false;
	}

	bool IniFile::GetInteger(const std::string& Section, const std::string& Key, int32& Value) const
	{
		std::string RawIniValue{};
		if (GetString(Section, Key, RawIniValue))
		{
			Value = std::stoi(RawIniValue);
			return true;
		}

		return false;
	}

	bool IniFile::GetFloat(const std::string& Section, const std::string& Key, float& Value) const
	{
		std::string RawIniValue{};
		if (GetString(Section, Key, RawIniValue))
		{
			Value = std::stof(RawIniValue);
			return true;
		}

		return false;
	}

	bool IniFile::GetBoolean(const std::string& Section, const std::string& Key, bool& bValue) const
	{
		std::string RawIniValue{};
		if (GetString(Section, Key, RawIniValue))
		{
			StringToLowercase(RawIniValue);
			bValue = RawIniValue == "true";
			return true;
		}

		return false;
	}

	bool IniFile::GetVector2D(const std::string& Section, const std::string& Key, Math::Vec2D& Value) const
	{
		std::string RawIniValue{};
		if (GetString(Section, Key, RawIniValue))
		{
			// remove ( )
			RawIniValue.erase(0);
			RawIniValue.pop_back();

			TArray<std::string> VectorComponents{};
			SplitString(RawIniValue, ',', VectorComponents);

			__ASSERT(VectorComponents.Size() == 2, "IniFile::GetVector2D() > Requested Key is not a Vector2D");

			for (SIZE_T I{}; I < VectorComponents.Size(); ++I)
			{
				std::string& Component{ VectorComponents[I] };
				Component.erase(std::remove_if(Component.begin(), Component.end(), [](const char C)->bool
					{
						return std::isspace(C);
					}), Component.end());

				Value[static_cast<int32>(I)] = std::stod(Component);
			}

			return true;
		}

		return false;
	}

	bool IniFile::GetVector3D(const std::string& Section, const std::string& Key, Math::Vec3D& Value) const
	{
		std::string RawIniValue{};
		if (GetString(Section, Key, RawIniValue))
		{
			// remove ( )
			RawIniValue.erase(0, 1);
			RawIniValue.pop_back();

			TArray<std::string> VectorComponents{};
			SplitString(RawIniValue, ',', VectorComponents);

			__ASSERT(VectorComponents.Size() == 3, "IniFile::GetVector3D() > Requested Key is not a Vector2D");

			for (SIZE_T I{}; I < VectorComponents.Size(); ++I)
			{
				std::string& Component{ VectorComponents[I] };
				Component.erase(std::remove_if(Component.begin(), Component.end(), [](const char C)->bool
					{
						return std::isspace(C);
					}), Component.end());

				Value[static_cast<int32>(I)] = std::stod(Component);
			}

			return true;
		}

		return false;
	}

	void IniFile::ParseIniFile(const std::string_view FileContents)
	{
		std::string Line{};
		std::stringstream Stream{};

		Stream << FileContents;

		bool bIsInSection{};
		std::string CurrentSection{};

		while (std::getline(Stream, Line))
		{
			TrimWhiteSpace(Line);

			if (Line.empty() || IsLineComment(Line))
			{
				continue;
			}

			if (IsLineSection(Line))
			{
				KeyValuePairs.AddEmptyKeyValue(CurrentSection = Line.substr(Line.find('[') + 1, Line.find(']') - 1));
				bIsInSection = true;
			}
			else
			{
				__ASSERT(bIsInSection, "Cannot have ini keys before section declaration");

				const int32 EqualsPos{ static_cast<int32>(Line.find('=')) };
				const std::string Key{ Line.substr(0, EqualsPos) };
				std::string Value{ Line.substr(EqualsPos + 1) };

				// if Value is a string, we need to get rid of the quotation marks
				if (Value[0] == '"')
				{
					Value.erase(Value.begin());
					Value.pop_back();
				}

				KeyValuePairs[CurrentSection][Key] = Value;
			}
		}
	}
}