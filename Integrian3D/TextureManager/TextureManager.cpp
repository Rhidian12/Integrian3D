#include "TextureManager.h"
#include "../Texture/Texture.h"

namespace Integrian3D
{
	TextureManager& TextureManager::GetInstance()
	{
		if (!Instance)
		{
			Instance = std::make_unique<TextureManager>();
		}

		return *Instance.get();
	}

	void TextureManager::AddTexture(const std::string& name, const std::string& filePath)
	{
		Textures.insert(std::make_pair(name, std::make_unique<Texture>(filePath)));
	}

	Texture* TextureManager::GetTexture(const std::string_view name) const
	{
		const auto cIt{ Textures.find(name.data()) };

		__ASSERT(cIt != Textures.cend() && "TextureManager::GetTexture() > Name was not added");

		return cIt->second.get();
	}
}