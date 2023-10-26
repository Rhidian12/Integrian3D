#include "Shader.h"

#include "IO/File/File.h"
#include "IO/PathUtils.h"
#include "Thread/ThreadUtils.h"

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

	Shader::Shader(const std::string& VertexShaderPath, const std::string& FragmentShaderPath)
		: ProgramID{ ShaderErrorValue }
		, VertexShader{ VertexShaderPath, IO::OpenMode::OpenAlways, IO::FileMode::ASCII, true }
		, FragmentShader{ FragmentShaderPath, IO::OpenMode::OpenAlways, IO::FileMode::ASCII, true }
	{
		IASSERT_MSG(PathUtils::GetExtension(VertexShaderPath) == ".vert", "VertexShader file must have .vert extension! Given file: {}", VertexShaderPath);
		IASSERT_MSG(PathUtils::GetExtension(FragmentShaderPath) == ".frag", "FragmentShader file must have .frag extension! Given file: {}", FragmentShaderPath);

		VertexShader.GetOnFileChangedDelegate().Bind(std::bind(&Shader::OnShaderChanged, this, std::placeholders::_1));
		FragmentShader.GetOnFileChangedDelegate().Bind(std::bind(&Shader::OnShaderChanged, this, std::placeholders::_1));

		SetupShaders();
	}

	Shader::~Shader()
	{
		if (ProgramID != ShaderErrorValue)
		{
			glDeleteProgram(ProgramID);

			glDeleteShader(VertexShaderID);
			glDeleteShader(FragmentShaderID);
		}
	}

	Shader::Shader(Shader&& other) noexcept
		: ProgramID{ I_MOVE(other.ProgramID) }
		, VertexShader{ I_MOVE(other.VertexShader) }
		, FragmentShader{ I_MOVE(other.FragmentShader) }
		, VertexShaderID{ I_MOVE(other.VertexShaderID) }
		, FragmentShaderID{ I_MOVE(other.FragmentShaderID) }
	{
		other.ProgramID = ShaderErrorValue;
		other.VertexShaderID = ShaderErrorValue;
		other.FragmentShaderID = ShaderErrorValue;
	}

	Shader& Shader::operator=(Shader&& other) noexcept
	{
		ProgramID = I_MOVE(other.ProgramID);
		VertexShader = I_MOVE(other.VertexShader);
		FragmentShader = I_MOVE(other.FragmentShader);
		VertexShaderID = I_MOVE(other.VertexShaderID);
		FragmentShaderID = I_MOVE(other.FragmentShaderID);

		other.ProgramID = ShaderErrorValue;
		other.VertexShaderID = ShaderErrorValue;
		other.FragmentShaderID = ShaderErrorValue;

		return *this;
	}

	void Shader::Activate() const
	{
		__CHECK(ProgramID != ShaderErrorValue);
		glUseProgram(ProgramID);
	}

	void Shader::HotReloadShaders()
	{
		if (!(bReloadVertexShader || bReloadFragmentShader))
		{
			return;
		}

		if (bReloadVertexShader)
		{
			glDetachShader(ProgramID, VertexShaderID);
			glDeleteShader(VertexShaderID);
			VertexShaderID = ShaderErrorValue;
			CompileShader(ShaderType::VertexShader);
		}
		if (bReloadFragmentShader)
		{
			glDetachShader(ProgramID, FragmentShaderID);
			glDeleteShader(FragmentShaderID);
			FragmentShaderID = ShaderErrorValue;
			CompileShader(ShaderType::FragmentShader);
		}

		CompileShader(ShaderType::VertexShader);
		CompileShader(ShaderType::FragmentShader);
		CreateProgram();

		bReloadVertexShader = 0;
		bReloadFragmentShader = 0;
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

	void Shader::SetupShaders()
	{
		CompileShader(ShaderType::VertexShader);
		CompileShader(ShaderType::FragmentShader);

		CreateProgram();
	}

	void Shader::OnShaderChanged(const std::string& Filepath)
	{
		if (Filepath == VertexShader.GetFilepath())
		{
			bReloadVertexShader = 1;
		}
		else if (Filepath == FragmentShader.GetFilepath())
		{
			bReloadFragmentShader = 1;
		}
	}

	void Shader::CompileShader(const ShaderType ShaderType)
	{
		uint32 CreateShaderID{};
		IO::File* ShaderFile{};
		uint32* ShaderID{};
		std::string_view ShaderTypeName{};

		switch (ShaderType)
		{
		case ShaderType::VertexShader:
			CreateShaderID = GL_VERTEX_SHADER;
			ShaderFile = &VertexShader;
			ShaderTypeName = "Vertex Shader";
			ShaderID = &VertexShaderID;
			break;
		case ShaderType::FragmentShader:
			CreateShaderID = GL_FRAGMENT_SHADER;
			ShaderFile = &FragmentShader;
			ShaderTypeName = "Fragment Shader";
			ShaderID = &FragmentShaderID;
			break;
		default:
			IASSERT(false);
		}

		// Generate Shader ID
		*ShaderID = glCreateShader(CreateShaderID);
		const uint32 ActualShaderID = *ShaderID;

		// Attach the Shader code to the ID and compile it
		const char* pShaderSource{ ShaderFile->GetFileContents().data() };
		glShaderSource(ActualShaderID, 1, &pShaderSource, nullptr);
		glCompileShader(ActualShaderID);

		// Check if the compilation succeeded
		int success{};
		char infoLog[512]{};
		glGetShaderiv(ActualShaderID, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(ActualShaderID, 512, nullptr, infoLog);
			LOG(ShaderLog, LogErrorLevel::Fatal, "{} Shader compilation failed: {}", ShaderTypeName, infoLog);
		}
	}

	void Shader::CreateProgram()
	{
		if (ProgramID != ShaderErrorValue)
		{
			glDeleteProgram(ProgramID);
		}

		ProgramID = glCreateProgram();

		// Link all previously created shaders to the shader program
		glAttachShader(ProgramID, VertexShaderID);
		glAttachShader(ProgramID, FragmentShaderID);
		glLinkProgram(ProgramID);

		// Check if the shader linking succeeded
		int success{};
		char infoLog[512]{};
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(ProgramID, 512, nullptr, infoLog);
			LOG(ShaderLog, LogErrorLevel::Fatal, "Shader program linking failed: {}", infoLog);
		}
	}
}