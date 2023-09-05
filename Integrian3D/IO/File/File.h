#pragma once

#include "../../EngineConstants.h"
#include "../../Array/Array.h"
#include "../FileMode.h"
#include "../OpenMode.h"
#include "../IOUtils.h"

#include <string>
#include <string_view>

#ifdef _WIN32
#	pragma warning ( push )
#	pragma warning ( disable : 4005 ) /* warning C4005: 'APIENTRY': macro redefinition */ 
#		define WIN32_LEAN_AND_MEAN
#		include <Windows.h>
#	pragma warning ( pop )
#endif

namespace Integrian3D::IO
{
	/// <summary>
	/// Minimal RAII file abstraction.
	/// </summary>
	class File final
	{
	public:
		File(const std::string_view Filepath, const OpenMode OpenMode, const FileMode Mode);
		~File();

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
		#pragma endregion

	private:
		void CloseHandle();
		void* OpenFile(const OpenMode OpenMode) const;

		std::string_view Filepath;
		void* Handle;
		int32 Filesize;
		FileMode Mode;
	};

	#pragma region operator<<
	template<typename T, std::enable_if_t<bIsInteger<T>, bool>>
	inline File& File::operator<<(T Val)
	{
		__CHECK(Handle != nullptr && Handle != INVALID_HANDLE_VALUE);

		if (Mode == FileMode::ASCII)
		{
			T Temp{ Val };
			int32_t NumberOfDigits{};
			while (Temp)
			{
				++NumberOfDigits;

				Temp /= 10;
			}

			constexpr static int32_t BufferLength{ 16 };
			__ASSERT(NumberOfDigits < BufferLength, "File::operator<< integer too long to be printed, max number of digits allowed: %i", BufferLength);

			char Buffer[BufferLength]{};

			for (int i{}; Val; ++i)
			{
				Buffer[i] = Val % 10 + '0';

				Val /= 10;
			}

			// we insert them backwards, so swap them around
			for (int32_t i{}; i < NumberOfDigits / 2; ++i)
			{
				std::swap(Buffer[i], Buffer[NumberOfDigits - i - 1]);
			}

			if (WriteFile(Handle, Buffer, static_cast<DWORD>(NumberOfDigits), nullptr, nullptr) == 0)
			{
				LOG(File, Warning, "File::operator<< could not write to file %s", Filepath);
			}
			else
			{
				Filesize += NumberOfDigits;
			}
		}
		else
		{
			static constexpr int32_t Size{ sizeof(T) };
			char Buffer[Size]{};

			for (int32_t i{}; i < Size; ++i)
			{
				Buffer[i] = *(reinterpret_cast<const char*>(&Val) + i);
			}

			if (WriteFile(Handle, Buffer, static_cast<DWORD>(Size), nullptr, nullptr) == 0)
			{
				LOG(File, Warning, "File::operator<< could not write to file %s", Filepath);
			}
			else
			{
				Filesize += Size;
			}
		}

		return *this;
	}

	template<typename T, std::enable_if_t<bIsCharacter<T>, bool>>
	inline File& File::operator<<(T Val)
	{
		__CHECK(Handle != nullptr && Handle != INVALID_HANDLE_VALUE);

		if (WriteFile(Handle, &Val, 1, nullptr, nullptr) == 0)
		{
			LOG(File, Warning, "File::operator<< could not write to file %s", Filepath);
		}
		else
		{
			++Filesize;
		}

		return *this;
	}

	template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool>>
	inline File& File::operator<<(T Val)
	{
		__CHECK(Handle != nullptr && Handle != INVALID_HANDLE_VALUE);

		if (Mode == FileMode::ASCII)
		{
			// print whole part
			*this << static_cast<int32_t>(Val) << '.';

			// print decimal part
			T Whole{};
			T Fractional = std::modf(Val, &Whole);

			// [TODO]: Make precision variable. Macro? Config file?
			constexpr static int32_t Precision{ 2 };
			constexpr static int32_t Base{ 10 };
			Fractional *= static_cast<T>(pow(Base, Precision));

			*this << static_cast<int32_t>(Fractional);
		}
		else
		{
			static constexpr int32_t Size{ sizeof(T) };
			char Buffer[Size]{};

			for (int32_t i{}; i < Size; ++i)
			{
				Buffer[i] = *(reinterpret_cast<const char*>(&Val) + i);
			}

			if (WriteFile(Handle, Buffer, static_cast<DWORD>(Size), nullptr, nullptr) == 0)
			{
				LOG(File, Warning, "File::operator<< could not write to file %s", Filepath);
			}
			else
			{
				Filesize += Size;
			}
		}

		return *this;
	}

	template<typename T, std::enable_if_t<!std::is_fundamental_v<T>, bool>>
	inline File& File::operator<<(const T&)
	{
		static_assert(false, "File::operator<< non-fundamental types must define operator<<");

		return *this;
	}
	#pragma endregion

	#pragma region operator>>
	template<typename T, std::enable_if_t<bIsInteger<T>, bool>>
	inline const File& File::operator>>(T& Val) const
	{
		__CHECK(Handle != nullptr && Handle != INVALID_HANDLE_VALUE);

		if (Mode == FileMode::ASCII)
		{
			std::string String{};

			bool bContinue{ true };
			char CurrentChar{};
			while (bContinue)
			{
				if (ReadFile(Handle, &CurrentChar, 1, nullptr, nullptr) == 0)
				{
					bContinue = false;
					LOG(File, Warning, "File::operator>> could not read from file: %s", Filepath);
				}

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

			Val = std::stoi(String);
		}
		else
		{
			constexpr static int32 Size{ sizeof(T) };
			char Buffer[Size]{};

			if (ReadFile(Handle, Buffer, static_cast<DWORD>(Size), nullptr, nullptr) == 0)
			{
				LOG(File, Warning, "File::operator>> could not read from file: %s", Filepath);
			}

			Val = *reinterpret_cast<T*>(&Buffer[0]);
		}

		return *this;
	}

	template<typename T, std::enable_if_t<bIsCharacter<T>, bool>>
	inline const File& File::operator>>(T& Val) const
	{
		__CHECK(Handle != nullptr && Handle != INVALID_HANDLE_VALUE);

		if (ReadFile(Handle, &Val, 1, nullptr, nullptr) == 0)
		{
			LOG(File, Warning, "File::operator>> could not read from file: %s", Filepath);
		}

		return *this;
	}

	template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool>>
	inline const File& File::operator>>(T& Val) const
	{
		__CHECK(Handle != nullptr && Handle != INVALID_HANDLE_VALUE);

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

			if (ReadFile(Handle, Buffer, static_cast<DWORD>(Size), nullptr, nullptr) == 0)
			{
				LOG(File, Warning, "File::operator>> could not read from file: %s", Filepath);
			}

			Val = *reinterpret_cast<T*>(&Buffer[0]);
		}

		return *this;
	}

	template<typename T, std::enable_if_t<!std::is_fundamental_v<T>, bool>>
	inline const File& File::operator>>(T&) const
	{
		static_assert(false, "File::operator>> non-fundamental types must define operator>>");

		return *this;
	}
	#pragma endregion
}