#pragma once

#include "../../EngineConstants.h"
#include "../../Array/Array.h"
#include "../FileMode.h"
#include "../OpenMode.h"

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
		TArray<char> GetFileContents() const;

		template<typename T, std::enable_if_t<std::is_fundamental_v<T>, bool> = true>
		File& operator<<(const T& Val);

		template<typename T, std::enable_if_t<!std::is_fundamental_v<T>, bool> = true>
		File& operator<<(const T& Val);

		File& operator<<(const TArray<char>& SerializedData);

	private:
		void CloseHandle();

		std::string_view Filepath;
		void* Handle;
		int64_t Filesize;
		FileMode Mode;
	};

	template<typename T, std::enable_if_t<std::is_fundamental_v<T>, bool>>
	inline File& File::operator<<(const T& Val)
	{
		__ASSERT(Mode == FileMode::ASCII, "File::operator<< is only available on ASCII files");
		constexpr static int32_t Size{ sizeof(Val) };

		char Buffer[Size]{};
		memcpy(Buffer, &Val, Size);

		if (WriteFile(Handle, Buffer, Size, nullptr, nullptr) == 0)
		{
			LOG(File, Warning, "File::operator<< could not write to file %s", Filepath);
		}
		else
		{
			Filesize += Size;
		}

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