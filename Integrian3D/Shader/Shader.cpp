#include "Shader.h"

#include "IO/File/File.h"
#include "IO/PathUtils.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#pragma warning( push )
#pragma warning( disable:4201 )
#	include <gtc/type_ptr.hpp>
#pragma warning( pop )

#undef max

namespace Integrian3D
{
	static constexpr uint32 ShaderErrorValue = std::numeric_limits<uint32>::max();

	Shader::Shader()
		: ProgramID{ ShaderErrorValue } 
	{}

	Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
		: ProgramID{ ShaderErrorValue }
	{
		SetupShaders(vertexShaderPath, fragmentShaderPath);
	}

	Shader::~Shader()
	{
		if (ProgramID != ShaderErrorValue)
		{
			glDeleteProgram(ProgramID);
		}
	}

	Shader::Shader(Shader&& other) noexcept
		: ProgramID{ __MOVE(other.ProgramID) }
	{
		other.ProgramID = ShaderErrorValue;
	}

	Shader& Shader::operator=(Shader&& other) noexcept
	{
		ProgramID = __MOVE(other.ProgramID);

		other.ProgramID = ShaderErrorValue;

		return *this;
	}

	void Shader::SetShaders(const std::string& VertexShader, const std::string& FragmentShader)
	{
		SetupShaders(VertexShader, FragmentShader);
	}

	void Shader::Activate() const
	{
		__CHECK(ProgramID != ShaderErrorValue);
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

	void Shader::SetVec3(const std::string_view Name, const Math::Vec3D& Vec) const
	{
		glUniform3f(glGetUniformLocation(ProgramID, Name.data()), Vec.x, Vec.y, Vec.z);
	}

	void Shader::SetupShaders(const std::string& VertexShader, const std::string& FragmentShader)
	{
		IASSERT_MSG(PathUtils::GetExtension(VertexShader) == ".vert", "VertexShader file must have .vert extension! Given file: {}", VertexShader);
		IASSERT_MSG(PathUtils::GetExtension(FragmentShader) == ".frag", "FragmentShader file must have .frag extension! Given file: {}", FragmentShader);

		uint32 vertexShaderID{}, fragmentShaderID{};

		/* Get Vertex Shader */
		{
			const IO::File vertexShader{ VertexShader, IO::OpenMode::OpenExisting, IO::FileMode::ASCII };

			/* Generate VertexShader ID */
			vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

			/* Attach the Vertex Shader code to the ID and compile it */
			const char* pShaderSource{ vertexShader.GetFileContents().data() };
			glShaderSource(vertexShaderID, 1, &pShaderSource, nullptr);
			glCompileShader(vertexShaderID);

			/* Check if the compilation succeeded */
			int success{};
			char infoLog[512]{};
			glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);

			if (!success)
			{
				glGetShaderInfoLog(vertexShaderID, 512, nullptr, infoLog);
				LOG(ShaderLog, LogErrorLevel::Fatal, "Vertex Shader compilation failed: {}", infoLog);
			}
		}

		/* Get Fragment Shader */
		{
			const IO::File fragmentShader{ FragmentShader, IO::OpenMode::OpenExisting, IO::FileMode::ASCII };

			/* Generate Frament Shader ID */
			fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

			/* Attach the Fragment Shader code to the ID and compile it */
			const char* pShaderSource{ fragmentShader.GetFileContents().data() };
			glShaderSource(fragmentShaderID, 1, &pShaderSource, nullptr);
			glCompileShader(fragmentShaderID);

			/* Check if the compilation succeeded */
			int success{};
			char infoLog[512]{};
			glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);

			if (!success)
			{
				glGetShaderInfoLog(fragmentShaderID, 512, nullptr, infoLog);
				LOG(ShaderLog, LogErrorLevel::Fatal, "Fragment Shader compilation failed: {}", infoLog);
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
				LOG(ShaderLog, LogErrorLevel::Fatal, "Shader program linking failed: {}", infoLog);
			}
		}

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}
}