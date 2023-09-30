#include "Texture/DiffuseTexture.h"

#include "DebugUtility/DebugUtility.h"

#include <raylib.h>

namespace Integrian3D
{
	DiffuseTexture::DiffuseTexture(const std::string_view filePath)
		: TextureImplementation{}
	{
		TextureImplementation = new Texture{ LoadTexture(filePath.data()) };
	}

	DiffuseTexture::~DiffuseTexture()
	{
		__DELETE(TextureImplementation);
	}

	DiffuseTexture::DiffuseTexture(DiffuseTexture&& other) noexcept
		: TextureImplementation{ __MOVE(other.TextureImplementation) }
	{
		other.TextureImplementation = nullptr;
	}

	DiffuseTexture& DiffuseTexture::operator=(DiffuseTexture&& other) noexcept
	{
		if (TextureImplementation)
		{
			__DELETE(TextureImplementation);
		}

		TextureImplementation = __MOVE(other.TextureImplementation);
		other.TextureImplementation = nullptr;

		return *this;
	}

	// void DiffuseTexture::UseTexture(Material* Material) const
	// {
	// 	SetMaterialTexture(Material, MATERIAL_MAP_DIFFUSE, *TextureImplementation);
	// }
}