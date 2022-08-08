#pragma once

#include <string_view>

namespace Integrian3D
{
	class Shader final
	{
	public:
		explicit Shader(const std::string& filePath);


	private:
		uint32_t ShaderID;
	};
}