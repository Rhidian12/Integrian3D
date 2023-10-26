#pragma once

#include "EngineConstants.h"

#include <string_view> /* std::string_view */

namespace Integrian3D
{
	class Texture final
	{
	public:
		explicit Texture(const std::string_view filePath);
		~Texture();

		Texture(const Texture&) noexcept = delete;
		Texture(Texture&& other) noexcept;
		Texture& operator=(const Texture&) noexcept = delete;
		Texture& operator=(Texture&& other) noexcept;

		__NODISCARD int GetWidth() const { return Width; }
		__NODISCARD int GetHeight() const { return Height; }
		__NODISCARD uint32_t GetTextureID() const { return TextureID; }

	private:
		int Width;
		int Height;
		uint32_t TextureID;
	};
}