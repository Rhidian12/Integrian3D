#include "IO/Ini/IniFile.h"

#include <algorithm>
#include <sstream>

namespace Integrian3D
{
	namespace
	{
		__INLINE static bool IsCharWhitespace(const char Char)
		{
			return std::isspace(Char) != 0;
		}

		static bool IsLineComment(const std::string_view Line)
		{
			for (const char C : Line)
			{
				if (IsCharWhitespace(C))
				{
					continue;
				}

				if (C == ';')
				{
					return true;
				}
				else
				{
					return false;
				}
			}

			return false;
		}

		static bool IsLineSection(const std::string_view Line, SIZE_T& Start)
		{
			Start = std::string::npos;

			for (SIZE_T I{}; I < Line.size(); ++I)
			{
				const char Char{ Line[I] };

				if (IsCharWhitespace(Char))
				{
					continue;
				}

				if (Char == '[')
				{
					Start = I;
					return true;
				}
				else
				{
					return false;
				}
			}

			return false;
		}

		__INLINE static void StringToLowercase(std::string& String)
		{
			std::transform(String.begin(), String.end(), String.begin(), [](const char Char)->char
				{
					return std::tolower(Char);
				}); 
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

	void IniFile::ParseIniFile(const std::string_view FileContents)
	{
		std::string Line{};
		std::stringstream Stream{};

		Stream << FileContents;

		bool bIsInSection{};
		std::string CurrentSection{};

		while (std::getline(Stream, Line))
		{
			if (Line.empty() || IsLineComment(Line))
			{
				continue;
			}

			SIZE_T SectionStart{};
			if (IsLineSection(Line, SectionStart))
			{
				KeyValuePairs.AddEmptyKeyValue(CurrentSection = Line.substr(SectionStart + 1, Line.find(']')));
				bIsInSection = true;
			}
			else
			{
				__ASSERT(bIsInSection, "Cannot have ini keys before section declaration");

				const int32 EqualsPos{ static_cast<int32>(Line.find('=')) };
				const std::string Key{ Line.substr(0, EqualsPos) };
				const std::string Value{ Line.substr(EqualsPos + 1) };

				KeyValuePairs[CurrentSection][Key] = Value;
			}
		}
	}
}