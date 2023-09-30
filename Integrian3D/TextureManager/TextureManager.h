#pragma once

#include "EngineConstants.h"
#include "Memory/UniquePtr.h"

#include <unordered_map> /* std::unordered_map */

namespace Integrian3D
{
	class DiffuseTexture;

	class TextureManager final
	{
	public:
		static TextureManager& GetInstance();

		void AddTexture(const std::string& name, const std::string& filePath);

		__NODISCARD const DiffuseTexture* const GetTexture(const std::string_view name) const;

	private:
		using MapType = std::unordered_map<std::string, UniquePtr<DiffuseTexture>>;

		TextureManager() = default;

		friend std::unique_ptr<TextureManager> std::make_unique();
		inline static std::unique_ptr<TextureManager> Instance{};

		MapType Textures;
	};
}