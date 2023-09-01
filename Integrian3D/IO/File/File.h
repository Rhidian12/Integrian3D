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
	namespace Detail
	{
		struct NoLShiftOperator final {};
		template<typename T, typename U, typename = std::enable_if_t<!std::is_fundamental_v<T>>> NoLShiftOperator operator<<(T&, const U&);
	}

	/// <summary>
	/// Minimal RAII file abstraction.
	/// </summary>
	class File final
	{
	private:
		template<typename T>
		constexpr static bool bDefinesLShiftOp = !std::is_same_v<decltype(*reinterpret_cast<File*>(0) << *reinterpret_cast<T*>(0)), Detail::NoLShiftOperator>;
		template<typename T>
		constexpr static bool bLShiftOpRetVal = std::is_same_v<decltype(*reinterpret_cast<File*>(0) << *reinterpret_cast<T*>(0)), File&>;

	public:
		File(const std::string_view Filepath, const OpenMode OpenMode, const FileMode Mode);
		~File();

		File(const File&) noexcept = delete;
		File(File&& Other) noexcept;
		File& operator=(const File&) noexcept = delete;
		File& operator=(File&& Other) noexcept;

		const std::string_view GetFilepath() const;
		const int64_t GetFilesize() const;
		std::string GetFileContents() const;

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

	private:
		void CloseHandle();
		void* OpenFile(const OpenMode OpenMode) const;

		std::string_view Filepath;
		void* Handle;
		int64_t Filesize;
		FileMode Mode;
	};

	template<typename T, std::enable_if_t<bIsInteger<T>, bool>>
	inline File& File::operator<<(T Val)
	{
		__ASSERT(Mode == FileMode::ASCII, "File::operator<< is only available on ASCII files");

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

		return *this;
	}

	template<typename T, std::enable_if_t<bIsCharacter<T>, bool>>
	inline File& File::operator<<(T Val)
	{
		__ASSERT(Mode == FileMode::ASCII, "File::operator<< is only available on ASCII files");

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
		__ASSERT(Mode == FileMode::ASCII, "File::operator<< is only available on ASCII files");

		// print whole part
		*this << static_cast<int32_t>(Val) << '.';

		// print decimal part
		float Whole{};
		float Fractional = std::modf(Val, &Whole);

		constexpr static int32_t Precision{ 2 };
		constexpr static int32_t Base{ 10 };
		Fractional *= static_cast<T>(pow(Base, Precision));

		*this << static_cast<int32_t>(Fractional);

		return *this;
	}

	template<typename T, std::enable_if_t<!std::is_fundamental_v<T>, bool>>
	inline File& File::operator<<(const T&)
	{
		static_assert(bDefinesLShiftOp<T>, "File::operator<< non-fundamental types must define operator<<");
		static_assert(bLShiftOpRetVal<T>, "File::operator<< operator<< must return TArray<char>");

		return *this;
	}
}