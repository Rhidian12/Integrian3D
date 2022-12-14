#include "Shader.h"

#include "../IO/ASCII/FileReader.h"
#include "../DebugUtility/DebugUtility.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#pragma warning( push )
#pragma warning( disable:4201 )
#include <gtc/type_ptr.hpp>
#pragma warning( pop )

namespace Integrian3D
{
	Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
		: ProgramID{ std::numeric_limits<uint32_t>::max() }
	{
		uint32_t vertexShaderID{}, fragmentShaderID{};

		/* Get Vertex Shader */
		{
			const IO::FileReader reader{ vertexShaderPath };

			/* Generate VertexShader ID */
			vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

			/* Attach the VertexShader code to the ID and compile it */
			const char* pShaderSource{ reader.GetFileContents().c_str() };
			glShaderSource(vertexShaderID, 1, &pShaderSource, nullptr);
			glCompileShader(vertexShaderID);

			/* Check if the compilation succeeded */
			int success{};
			char infoLog[512]{};
			glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);

			if (!success)
			{
				glGetShaderInfoLog(vertexShaderID, 512, nullptr, infoLog);
				Debug::LogError(std::string("Vertex Shader compilation failed: ") + infoLog, false);
			}
		}

		/* Get Fragment Shader */
		{
			const IO::FileReader reader{ fragmentShaderPath };

			/* Generate Frament Shader ID */
			fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

			/* Attach the VertexShader code to the ID and compile it */
			const char* pShaderSource{ reader.GetFileContents().c_str() };
			glShaderSource(fragmentShaderID, 1, &pShaderSource, nullptr);
			glCompileShader(fragmentShaderID);

			/* Check if the compilation succeeded */
			int success{};
			char infoLog[512]{};
			glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);

			if (!success)
			{
				glGetShaderInfoLog(fragmentShaderID, 512, nullptr, infoLog);
				Debug::LogError(std::string("Fragment Shader compilation failed: ") + infoLog, false);
			}
		}

		/* Create shader program */
		{
			ProgramID = glCreateProgram();

			/* Link all previously created shaders to the shader program */
			glAttachShader(ProgramID, vertexShaderID);
			glAttachShader(ProgramID, fragmentShaderID);
			glLinkProgram(ProgramID);

			/* Check if the shader linking succeeded */
			int success{};
			char infoLog[512]{};
			glGetProgramiv(ProgramID, GL_LINK_STATUS, &success);

			if (!success)
			{
				glGetProgramInfoLog(ProgramID, 512, nullptr, infoLog);
				Debug::LogError(std::string("Shader program linking failed: ") + infoLog, false);
			}
		}

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	Shader::~Shader()
	{
		if (ProgramID != std::numeric_limits<uint32_t>::max())
		{
			glDeleteProgram(ProgramID);
		}
	}

	Shader::Shader(Shader&& other) noexcept
		: ProgramID{ __MOVE(other.ProgramID) }
	{
		other.ProgramID = std::numeric_limits<uint32_t>::max();
	}

	Shader& Shader::operator=(Shader&& other) noexcept
	{
		ProgramID = __MOVE(other.ProgramID);

		other.ProgramID = std::numeric_limits<uint32_t>::max();

		return *this;
	}

	void Shader::Activate() const
	{
		__ASSERT(ProgramID != std::numeric_limits<uint32_t>::max() && "Shader::Activate() > No program set");
		glUseProgram(ProgramID);
	}

	void Shader::SetBool(const std::string_view name, const bool value) const
	{
		glUniform1i(glGetUniformLocation(ProgramID, name.data()), static_cast<int>(value));
	}

	void Shader::SetInt(const std::string_view name, const int value) const
	{
		glUniform1i(glGetUniformLocation(ProgramID, name.data()), value);
	}

	void Shader::SetFloat(const std::string_view name, const float value) const
	{
		glUniform1f(glGetUniformLocation(ProgramID, name.data()), value);
	}

	void Shader::SetMatrix(const std::string_view name, const glm::mat4& value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ProgramID, name.data()), 1, GL_FALSE, glm::value_ptr(value));
	}
}