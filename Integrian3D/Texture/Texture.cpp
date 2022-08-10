#include "Texture.h"
#include "../EngineConstants.h"
#include "../DebugUtility/DebugUtility.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../Libraries/stb/stb_image.h"

namespace Integrian3D
{
	Texture::Texture(const std::string_view filePath)
		: Width{}
		, Height{}
		, TextureID{}
	{
		/* Load image using stb */
		int nrOfChannels{};
		unsigned char* pData = stbi_load(filePath.data(), &Width, &Height, &nrOfChannels, 0);

		/* Generate texture ID */
		glGenTextures(1, &TextureID);

		/* Bind ID to texture */
		glBindTexture(GL_TEXTURE_2D, TextureID);

		/* Set wrapping and filtering options */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		/* Create Texture */
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData);

		/* Generate mipmaps */
		glGenerateMipmap(TextureID);

		/* free stb image */
		stbi_image_free(pData);
	}

	Texture::~Texture()
	{
		if (TextureID != std::numeric_limits<uint32_t>::max())
		{
			glDeleteTextures(1, &TextureID);
		}
	}

	Texture::Texture(Texture&& other) noexcept
		: Width{ std::move(other.Width) }
		, Height{ std::move(other.Height) }
		, TextureID{ std::move(other.TextureID) }
	{
		other.TextureID = std::numeric_limits<uint32_t>::max();
	}

	Texture& Texture::operator=(Texture&& other) noexcept
	{
		Width = std::move(other.Width);
		Height = std::move(other.Height);
		TextureID = std::move(other.TextureID);

		other.TextureID = std::numeric_limits<uint32_t>::max();

		return *this;
	}
}