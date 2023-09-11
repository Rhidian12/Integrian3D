#pragma once

#include "EngineConstants.h"

#include <string> /* std::string */
#include <assert.h> /* assert() */
#include <string_view> /* std::string_view */
#include <glm.hpp> /* GLM */

DECLARE_LOG_CATEGORY(Shader)

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

		void Activate() const;

		void SetBool(const std::string_view name, const bool value) const;
		void SetInt(const std::string_view name, const int value) const;
		void SetFloat(const std::string_view name, const float value) const;
		void SetMatrix(const std::string_view name, const glm::mat4& value) const;

	private:
		uint32_t ProgramID;
	};
}