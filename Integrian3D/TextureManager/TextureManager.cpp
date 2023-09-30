#include "TextureManager/TextureManager.h"
#include "Texture/DiffuseTexture.h"

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
		Textures.insert(std::make_pair(name, MakeUnique<DiffuseTexture>(filePath)));
	}

	const DiffuseTexture* const TextureManager::GetTexture(const std::string_view name) const
	{
		MapType::const_iterator CIt{ Textures.find(name.data()) };

		__ASSERT(CIt != Textures.cend(), "TextureManager::GetTexture() > Name %s cannot be found", name);

		return CIt->second.Get();
	}
}