#pragma once

#include "../../EngineConstants.h"

namespace Integrian3D::Memory
{
	template<typename ...>
	using Void = void;

	template<typename T, typename U, typename = void>
	struct _CanAllocate : public std::false_type {};

	template<typename T, typename U>
	struct _CanAllocate<T, U, Void<decltype(std::declval<T>().Allocate<U>(0))>> : public std::true_type {};

	template<typename T>
	class AllocatorTraits
	{
		using AllocatorType = T;
		using Value = typename T::Value;
		using Pointer = Value*;
		using ConstPointer = const Value*;
		using Diff = uint64_t;

		template<typename U>
		constexpr static auto CanAllocate = _CanAllocate<T, U>::value;
	};
}