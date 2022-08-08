#pragma once

#include <string> /* std::string */
#include <assert.h> /* assert() */

namespace Integrian3D
{
	class Shader final
	{
	public:
		Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
		~Shader();

		Shader(const Shader&) noexcept = delete;
		Shader(Shader&& other) noexcept;
		Shader& operator=(const Shader&) noexcept = delete;
		Shader& operator=(Shader&& other) noexcept;

		void Activate();

		uint32_t GetProgramID() const { return ProgramID; }

	private:
		uint32_t ProgramID;
	};
}