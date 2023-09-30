#pragma once

#include "EngineConstants.h"
#include "Memory/UniquePtr.h"

#include <string_view> /* std::string_view */

struct Texture;
namespace Integrian3D
{
	class DiffuseTexture final
	{
	public:
		explicit DiffuseTexture(const std::string_view filePath);
		~DiffuseTexture();

		DiffuseTexture(const DiffuseTexture&) noexcept = delete;
		DiffuseTexture(DiffuseTexture&& other) noexcept;
		DiffuseTexture& operator=(const DiffuseTexture&) noexcept = delete;
		DiffuseTexture& operator=(DiffuseTexture&& other) noexcept;

		// void UseTexture(struct Material* Material) const;

	private:
		Texture* TextureImplementation; // By exception, this is owned by DiffuseTexture
	};
}