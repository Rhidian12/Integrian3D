#pragma once

#include <string_view> /* std::string_view */

namespace Integrian3D
{
	class Texture final
	{
	public:
		explicit Texture(const std::string_view filePath);
		~Texture();

	private:
		int Width;
		int Height;
		uint32_t TextureID;
	};
}