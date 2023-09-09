#pragma once

#include "EngineConstants.h"
#include "Array/Array.h"
#include "IO/FileMode.h"
#include "IO/OpenMode.h"
#include "IO/IOUtils.h"
#include "Win32Utils/Win32Handle.h"

#include <string>
#include <string_view>

namespace Integrian3D::IO
{
	/// <summary>
	/// Minimal RAII file abstraction.
	/// </summary>
	class File final
	{
	public:
		File(const std::string_view Filepath, const OpenMode OpenMode, const FileMode Mode);

		File(const File&) noexcept = delete;
		File(File&& Other) noexcept;
		File& operator=(const File&) noexcept = delete;
		File& operator=(File&& Other) noexcept;

		void Seek(const int32 NewFilepointerPos) const;
		void Advance(const int32 AdvanceAmount) const;
		void Regress(const int32 RegressAmount) const;

		const std::string_view GetFilepath() const;
		std::string GetFileContents() const;
		int32 GetFilesize() const;
		int32 GetFilepointer() const;

		#pragma region operator<<
		template<typename T, std::enable_if_t<bIsInteger<T>, bool> = true>
		File& operator<<(T Val);

		template<typename T, std::enable_if_t<bIsCharacter<T>, bool> = true>
		File& operator<<(T Val);

		template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
		File& operator<<(T Val);

		template<typename T, std::enable_if_t<!std::is_fundamental_v<T>, bool> = true>
		File& operator<<(const T& Val);

		File& operator<<(const char* String);

		File& operator<<(const std::string& String);

		File& operator<<(const bool bValue);
		#pragma endregion

		#pragma region operator>>
		template<typename T, std::enable_if_t<bIsInteger<T>, bool> = true>
		const File& operator>>(T& Val) const;

		template<typename T, std::enable_if_t<bIsCharacter<T>, bool> = true>
		const File& operator>>(T& Val) const;

		template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
		const File& operator>>(T& Val) const;

		template<typename T, std::enable_if_t<!std::is_fundamental_v<T>, bool> = true>
		const File& operator>>(T& Val) const;

		const File& operator>>(std::string& String) const;

		const File& operator>>(bool& bValue) const;
		#pragma endregion

	private:
		void WriteToFile(const char* Buffer, const int32 BufferSize);
		char ReadCharacterFromFile() const;
		void ReadFromFile(char* Buffer, const int32 BufferSize) const;
		void* OpenFile(const OpenMode OpenMode) const;
		bool IsHandleValid() const;

		std::string_view Filepath;
		Win32Utils::Win32Handle Handle;
		int32 Filesize;
		FileMode Mode;
	};

	#pragma region operator<<
	template<typename T, std::enable_if_t<bIsInteger<T>, bool>>
	inline File& File::operator<<(T Val)
	{
		__CHECK(IsHandleValid());

		if (Mode == FileMode::ASCII)
		{
			*this << std::to_string(Val);
		}
		else
		{
			static constexpr int32_t Size{ sizeof(T) };
			char Buffer[Size]{};

			for (int32_t i{}; i < Size; ++i)
			{
				Buffer[i] = *(reinterpret_cast<const char*>(&Val) + i);
			}

			WriteToFile(Buffer, Size);
		}

		return *this;
	}

	template<typename T, std::enable_if_t<bIsCharacter<T>, bool>>
	inline File& File::operator<<(T Val)
	{
		__CHECK(IsHandleValid());

		WriteToFile(&Val, sizeof(T));

		return *this;
	}

	template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool>>
	inline File& File::operator<<(T Val)
	{
		__CHECK(IsHandleValid());

		if (Mode == FileMode::ASCII)
		{
			// print whole part
			*this << static_cast<int32>(Val) << '.';

			// print decimal part
			T Whole{};
			T Fractional = std::modf(Val, &Whole);

			// [TODO]: Make precision variable. Macro? Config file?
			constexpr static int32 Precision{ 2 };
			constexpr static int32 Base{ 10 };
			Fractional *= static_cast<T>(pow(Base, Precision));

			*this << static_cast<int32>(Fractional);
		}
		else
		{
			static constexpr int32 Size{ sizeof(T) };
			char Buffer[Size]{};

			for (int32 i{}; i < Size; ++i)
			{
				Buffer[i] = *(reinterpret_cast<const char*>(&Val) + i);
			}

			WriteToFile(Buffer, Size);
		}

		return *this;
	}
	#pragma endregion

	#pragma region operator>>
	template<typename T, std::enable_if_t<bIsInteger<T>, bool>>
	inline const File& File::operator>>(T& Val) const
	{
		__CHECK(IsHandleValid());

		if (Mode == FileMode::ASCII)
		{
			std::string String{};

			bool bContinue{ true };
			char CurrentChar{};
			while (bContinue)
			{
				CurrentChar = ReadCharacterFromFile();

				if (CurrentChar == '\n')
				{
					// Consume newline but stop reading numbers
					break;
				}

				const bool bIsDigit{ std::isdigit(CurrentChar) != 0 };
				bContinue = bIsDigit;

				if (bIsDigit)
				{
					String += CurrentChar;
				}
			}

			Val = static_cast<T>(std::stoi(String));
		}
		else
		{
			constexpr static int32 Size{ sizeof(T) };
			char Buffer[Size]{};

			ReadFromFile(Buffer, Size);

			Val = *reinterpret_cast<T*>(&Buffer[0]);
		}

		return *this;
	}

	template<typename T, std::enable_if_t<bIsCharacter<T>, bool>>
	inline const File& File::operator>>(T& Val) const
	{
		__CHECK(IsHandleValid());

		Val = ReadCharacterFromFile();

		return *this;
	}

	template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool>>
	inline const File& File::operator>>(T& Val) const
	{
		__CHECK(IsHandleValid());

		if (Mode == FileMode::ASCII)
		{
			int32 Whole{}, FractionalTemp{};
			*this >> Whole >> FractionalTemp;

			T Fractional{ static_cast<T>(FractionalTemp) };
			while (Fractional > static_cast<T>(1.f))
			{
				Fractional /= static_cast<T>(10.f);
			}

			Val = static_cast<T>(Whole) + Fractional;
		}
		else
		{
			constexpr static int32 Size{ sizeof(T) };
			char Buffer[Size]{};

			ReadFromFile(Buffer, Size);

			Val = *reinterpret_cast<T*>(&Buffer[0]);
		}

		return *this;
	}
	#pragma endregion
}