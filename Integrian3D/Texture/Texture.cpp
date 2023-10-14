#include "Texture.h"
#include "../EngineConstants.h"
#include "../DebugUtility/DebugUtility.h"
#include "../Renderer/Renderer.h"

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
		/* Generate texture ID */
		glGenTextures(1, &TextureID);

		/* Bind ID to texture */
		glBindTexture(GL_TEXTURE_2D, TextureID);

		/* Set wrapping and filtering options */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		/* Load image using stb */
		int nrOfChannels{};
		unsigned char* pData = stbi_load(filePath.data(), &Width, &Height, &nrOfChannels, 0);

		ICHECK_MSG(pData != nullptr, "Texture could not be loaded from filepath {}", filePath);

		/* Create Texture */
		const std::string_view fileExtension{ filePath.substr(filePath.find_last_of('.')) };
		if (fileExtension == ".png" || fileExtension == ".tiff")
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, pData);
		}

		/* Generate mipmaps */
		glGenerateTextureMipmap(TextureID);

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
		: Width{ __MOVE(other.Width) }
		, Height{ __MOVE(other.Height) }
		, TextureID{ __MOVE(other.TextureID) }
	{
		other.TextureID = std::numeric_limits<uint32_t>::max();
	}

	Texture& Texture::operator=(Texture&& other) noexcept
	{
		Width = __MOVE(other.Width);
		Height = __MOVE(other.Height);
		TextureID = __MOVE(other.TextureID);

		other.TextureID = std::numeric_limits<uint32_t>::max();

		return *this;
	}
}