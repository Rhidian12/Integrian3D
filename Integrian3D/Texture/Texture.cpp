#include "Texture.h"

#include "DebugUtility/DebugUtility.h"

#include <raylib.h>

namespace Integrian3D
{
	Texture::Texture(const std::string_view filePath)
		: TextureImplementation{}
	{
		TextureImplementation = MakeUnique<Texture>(LoadTexture(filePath.data()));
	}

	Texture::Texture(Texture&& other) noexcept
		: TextureImplementation{ __MOVE(other.TextureImplementation) }
	{}

	Texture& Texture::operator=(Texture&& other) noexcept
	{
		TextureImplementation = __MOVE(other.TextureImplementation);

		return *this;
	}

	void Texture::UseTexture(Material* Material) const
	{
		SetMaterialTexture(Material, MATERIAL_MAP_DIFFUSE, *TextureImplementation);
	}
}