#pragma once

#include "../../EngineConstants.h"

namespace Integrian3D::Memory
{
	namespace // internal linkage
	{
		template<typename ...>
		using Void = void;

		template<typename T, typename U, typename = void>
		struct _CanAllocate : public std::false_type {};

		template<typename T, typename U>
		struct _CanAllocate<T, U, Void<decltype(std::declval<T>().Allocate<U>(0, 0))>> : public std::true_type {};

		template<typename T, typename U, typename = void>
		struct _CanDeallocate : public std::false_type {};

		template<typename T, typename U>
		struct _CanDeallocate<T, U, Void<decltype(std::declval<T>().Deallocate<U>(nullptr))>> : public std::true_type {};

		template<typename T, typename = void>
		struct _HasData : public std::false_type {};

		template<typename T>
		struct _HasData<T, Void<decltype(std::declval<T>().Data())>> : public std::true_type {};
	}

	template<typename T>
	struct AllocatorTraits
	{
		using Type = T;

		template<typename U>
		constexpr static auto CanAllocate = _CanAllocate<T, U>::value;

		template<typename U>
		constexpr static auto CanDeallocate = _CanDeallocate<T, U>::value;

		constexpr static auto HasData = _HasData<T>::value;

		constexpr static auto IsCopyConstructable = std::is_copy_constructible_v<T>;
		constexpr static auto IsMoveConstructable = std::is_move_constructible_v<T>;

		constexpr static auto IsCopyAssignable = std::is_copy_assignable_v<T>;
		constexpr static auto IsMoveAssignable = std::is_move_assignable_v<T>;
	};
}