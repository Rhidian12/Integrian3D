#pragma once

#include "EngineConstants.h"
#include "Memory/UniquePtr.h"

#include <string_view> /* std::string_view */

struct Texture;
namespace Integrian3D
{
	class Texture final
	{
	public:
		explicit Texture(const std::string_view filePath);

		Texture(const Texture&) noexcept = delete;
		Texture(Texture&& other) noexcept;
		Texture& operator=(const Texture&) noexcept = delete;
		Texture& operator=(Texture&& other) noexcept;

		void UseTexture(struct Material* Material) const;

	private:
		UniquePtr<Texture> TextureImplementation;
	};
}