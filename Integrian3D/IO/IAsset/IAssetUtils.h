#pragma once

#include "../../EngineConstants.h"
#include "../../TemplateUtils/TemplateUtils.h"

#include "../Binary/BinaryReader.h"
#include "../Binary/BinaryWriter.h"

#include <type_traits>

namespace Integrian3D::IO
{
	namespace Detail
	{
		template<typename T, typename U, typename = void>
		struct _IAsset_HasDeserializeRet : public std::false_type {};

		template<typename T, typename U>
		struct _IAsset_HasDeserializeRet<T, U, TUtils::Void<std::is_same<decltype(std::declval<T>().Deserialize((BinaryReader&)BinaryReader())), U>>> : public std::true_type {};

		template<typename T, typename U, typename = void>
		struct _IAsset_HasDeserialize : public std::false_type {};

		template<typename T, typename U>
		struct _IAsset_HasDeserialize<T, U, TUtils::Void<decltype(std::declval<T>().Deserialize((BinaryReader&)BinaryReader(), (U&)U()))>> : public std::true_type {};

		template<typename T, typename U, typename = void>
		struct _IAsset_HasSerialize : public std::false_type {};

		template<typename T, typename U>
		struct _IAsset_HasSerialize<T, U, TUtils::Void<decltype(std::declval<T>().Serialize((BinaryWriter&)BinaryWriter(), U()))>> : public std::true_type {};
	}

	template<typename T, typename U>
	constexpr static auto IAsset_HasDeserializeRet{ Detail::_IAsset_HasDeserializeRet<T, U>::value };

	template<typename T, typename U>
	constexpr static auto IAsset_HasDeserialize{ Detail::_IAsset_HasDeserialize<T, U>::value };

	template<typename T, typename U>
	constexpr static auto IAsset_HasSerialize{ Detail::_IAsset_HasSerialize<T, U>::value };
}