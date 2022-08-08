#pragma once

#include <string> /* std::string */
#include <assert.h> /* assert() */
#include <string_view> /* std::string_view */

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

		void SetBool(const std::string_view name, const bool value) const;
		void SetInt(const std::string_view name, const int value) const;

	private:
		uint32_t ProgramID;
	};
}