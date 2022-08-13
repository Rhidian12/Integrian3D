#pragma once

#include "../EngineConstants.h"

#include <memory> /* std::unique_ptr */
#include <unordered_map> /* std::unordered_map */

namespace Integrian3D
{
	class TextureManager final
	{
	public:
		static TextureManager& GetInstance();

		void AddTexture(const std::string& name, const std::string& filePath);

		__NODISCARD class Texture* GetTexture(const std::string_view name) const;

	private:
		TextureManager() = default;

		friend std::unique_ptr<TextureManager> std::make_unique();
		inline static std::unique_ptr<TextureManager> Instance{};

		std::unordered_map<std::string, std::unique_ptr<class Texture>> Textures;
	};
}