#pragma once
#include "EngineConstants.h"
#include "Array/Array.h"

#include <string_view>

DEFINE_LOG_CATEGORY(Win32Log, Integrian3D::LogVerbosity::Verbose);

namespace Integrian3D::Win32Utils
{
	class Win32APICallInfo final
	{
	public:
		Win32APICallInfo(const std::string_view File, const int32 Line);
		Win32APICallInfo(const std::string_view File, const int32 Line, const DWORD ErrorToIgnore);
		Win32APICallInfo(const std::string_view File, const int32 Line, const TArray<DWORD>& ErrorsToIgnore);

		bool GetSuccess() const;

	private:
		template<typename Func>
		friend Win32APICallInfo Win32APICall(const Func& Function, const std::string_view File, const int32 Line);
		template<typename Func>
		friend Win32APICallInfo Win32APICall_IgnoreError(const Func& Function, const DWORD ErrorToIgnore, const std::string_view File, const int32 Line);
		template<typename Func>
		friend Win32APICallInfo Win32APICall_IgnoreErrors(const Func& Function, const TArray<DWORD>& ErrorsToIgnore, const std::string_view File, const int32 Line);
		template<typename T, typename Func>
		friend T Win32APICall_RV(const Func& Function, const std::string_view File,const int32 Line);
		template<typename T, typename Func>
		friend T Win32APICall_RV_IgnoreError(const Func& Function, const DWORD ErrorToIgnore, const std::string_view File, const int32 Line);
		template<typename T, typename Func>
		friend T Win32APICall_RV_IgnoreErrors(const Func& Function, const TArray<DWORD>& ErrorsToIgnore, const std::string_view File, const int32 Line);

		void LogError() const;

		std::string_view File;
		int32 Line;
		DWORD Result;
	};

	template<typename Func>
	Win32APICallInfo Win32APICall(const Func& Function, const std::string_view File, const int32 Line)
	{
		Function();
		const Win32APICallInfo CallInfo{ File, Line };

		if (!CallInfo.GetSuccess())
		{
			CallInfo.LogError();
		}

		return CallInfo;
	}

	template<typename Func>
	Win32APICallInfo Win32APICall_IgnoreError(const Func& Function, const DWORD ErrorToIgnore, const std::string_view File, const int32 Line)
	{
		Function();
		const Win32APICallInfo CallInfo{ File, Line, ErrorToIgnore };

		if (!CallInfo.GetSuccess())
		{
			CallInfo.LogError();
		}

		return CallInfo;
	}

	template<typename Func>
	Win32APICallInfo Win32APICall_IgnoreErrors(const Func& Function, const TArray<DWORD>& ErrorsToIgnore, const std::string_view File, const int32 Line)
	{
		Function();
		const Win32APICallInfo CallInfo{ File, Line, ErrorsToIgnore };

		if (!CallInfo.GetSuccess())
		{
			CallInfo.LogError();
		}

		return CallInfo;
	}

	template<typename T, typename Func>
	T Win32APICall_RV(const Func& Function, const std::string_view File, const int32 Line)
	{
		const T Result = Function();
		const Win32APICallInfo CallInfo{ File, Line };

		if (!CallInfo.GetSuccess())
		{
			CallInfo.LogError();
		}

		return Result;
	}

	template<typename T, typename Func>
	T Win32APICall_RV_IgnoreError(const Func& Function, const DWORD ErrorToIgnore, const std::string_view File, const int32 Line)
	{
		const T Result = Function();
		const Win32APICallInfo CallInfo{ File, Line, ErrorToIgnore };

		if (!CallInfo.GetSuccess())
		{
			CallInfo.LogError();
		}

		return Result;
	}

	template<typename T, typename Func>
	T Win32APICall_RV_IgnoreErrors(const Func& Function, const TArray<DWORD>& ErrorsToIgnore, const std::string_view File, const int32 Line)
	{
		const T Result = Function();
		const Win32APICallInfo CallInfo{ File, Line, ErrorsToIgnore };

		if (!CallInfo.GetSuccess())
		{
			CallInfo.LogError();
		}

		return Result;
	}
}

#ifdef _WIN32
#	define CALL_WIN32(FunctionCall) Integrian3D::Win32Utils::Win32APICall([&](){ return FunctionCall; }, __FILE__, __LINE__)
#	define CALL_WIN32_IGNORE_ERROR(FunctionCall, ErrorToIgnore) Integrian3D::Win32Utils::Win32APICall_IgnoreError([&](){ return FunctionCall; }, ErrorToIgnore, __FILE__, __LINE__)
#	define CALL_WIN32_IGNORE_ERRORS(FunctionCall, ...) Integrian3D::Win32Utils::Win32APICall_IgnoreErrors([&](){ return FunctionCall; }, TArray<DWORD>{ __VA_ARGS__ }, __FILE__, __LINE__ );
#	define CALL_WIN32_RV(FunctionCall) Integrian3D::Win32Utils::Win32APICall_RV<decltype(FunctionCall)>([&](){ return FunctionCall; }, __FILE__, __LINE__)
#	define CALL_WIN32_RV_IGNORE_ERROR(FunctionCall, ErrorToIgnore) Integrian3D::Win32Utils::Win32APICall_RV_IgnoreError<decltype(FunctionCall)>([&](){ return FunctionCall; }, ErrorToIgnore, __FILE__, __LINE__)
#	define CALL_WIN32_RV_IGNORE_ERRORS(FunctionCall, ...) Integrian3D::Win32Utils::Win32APICall_RV_IgnoreErrors<decltype(FunctionCall)>([&](){ return FunctionCall; }, TArray<DWORD>{ __VA_ARGS__ }, __FILE__, __LINE__)
#endif	