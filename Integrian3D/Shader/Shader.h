#pragma once

#include <string_view>

namespace Integrian3D
{
	class Shader final
	{
	public:
		explicit Shader(const std::string& filePath);
		~Shader();

		Shader(const Shader&) noexcept = delete;
		Shader(Shader&& other) noexcept;
		Shader& operator=(const Shader&) noexcept = delete;
		Shader& operator=(Shader&& other) noexcept;

		uint32_t GetShaderID() const { return ShaderID; }

	private:
		uint32_t ShaderID;
	};
}