#pragma once

#include "../../EngineConstants.h"
#include "../../Array/Array.h"
#include "../IOUtils.h"
#include "../../TemplateUtils/TemplateUtils.h"

#pragma region Serializers
template<typename T, std::enable_if_t<!Integrian3D::IO::IsPod<T>, bool> = true>
__NODISCARD __INLINE Integrian3D::TArray<char> Serialize(const T&)
{
	static_assert(Integrian3D::TUtils::template AlwaysFalse<T>, "Serialize<T>() > Must be implemented!");
}

template<typename T, std::enable_if_t<Integrian3D::IO::IsPod<T>, bool> = true>
__NODISCARD __INLINE Integrian3D::TArray<char> Serialize(const T& data)
{
	Integrian3D::TArray<char> arr{};

	for (uint32_t i{}; i < sizeof(T); ++i)
		arr.Add(*(reinterpret_cast<const char*>(&data) + i));

	return arr;
}

template<>
__NODISCARD __INLINE Integrian3D::TArray<char> Serialize<std::string>(const std::string& data)
{
	Integrian3D::TArray<char> arr{ Serialize(static_cast<uint32_t>(data.size())) };

	for (const char c : data)
		arr.Add(c);

	return arr;
}
#pragma endregion

#pragma region Deserializers
#pragma region OutParamT
template<typename T, std::enable_if_t<!Integrian3D::IO::IsPod<T>, bool> = true>
__NODISCARD __INLINE void Deserialize(const Integrian3D::TArray<char>&, T&)
{
	static_assert(Integrian3D::TUtils::AlwaysFalse<T>, "Deserialize<T>() > Must be implemented!");
}

template<typename T, std::enable_if_t<Integrian3D::IO::IsPod<T>, bool> = true>
__NODISCARD __INLINE void Deserialize(const Integrian3D::TArray<char>& data, T& val)
{
	using RawT = std::remove_cvref_t<T>;

	val = Integrian3D::IO::SwapEndianness<RawT>(*reinterpret_cast<const RawT*>(&data[0]));
}

template<>
__NODISCARD __INLINE void Deserialize<std::string>(const Integrian3D::TArray<char>& data, std::string& val)
{
	/* uint32_t is at the start of data for File::Read */

	for (uint32_t i{ sizeof(uint32_t) }; i < data.Size(); ++i)
		val.push_back(data[i]);
}
#pragma endregion

#pragma region ReturnT
template<typename T, std::enable_if_t<!Integrian3D::IO::IsPod<T>, bool> = true>
__NODISCARD __INLINE T Deserialize(const Integrian3D::TArray<char>&)
{
	static_assert(Integrian3D::TUtils::AlwaysFalse<T>, "Deserialize<T>() > Must be implemented!");
}

template<typename T, std::enable_if_t<Integrian3D::IO::IsPod<T>, bool> = true>
__NODISCARD __INLINE T Deserialize(const Integrian3D::TArray<char>& data)
{
	using RawT = std::remove_cvref_t<T>;

	T val{ Integrian3D::IO::SwapEndianness<RawT>(*reinterpret_cast<const RawT*>(&data[0])) };

	return val;
}

template<>
__NODISCARD __INLINE std::string Deserialize<std::string>(const Integrian3D::TArray<char>& data)
{
	std::string val{};

	/* uint32_t is at the start of data for File::Read */

	for (uint32_t i{ sizeof(uint32_t) }; i < data.Size(); ++i)
		val.push_back(data[i]);

	return val;
}
#pragma endregion
#pragma endregion
