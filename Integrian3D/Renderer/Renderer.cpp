#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../FileReader/FileReader.h"
#include "../DebugUtility/DebugUtility.h"

namespace Integrian3D
{
	Renderer::Renderer()
	{
		uint32_t vertexShaderID{}, fragmentShaderID{};

		/* Get Vertex shader */
		{
			const FileReader reader{ "Resources/VertexShader.txt" };

			/* Generate VertexShader ID */
			vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

			/* Attach the VertexShader code to the ID and compile it */
			const char* vertexShaderSource{ reader.GetFileContents().c_str() };
			glShaderSource(vertexShaderID, 1, &vertexShaderSource, nullptr);
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

		/* Get Fragment shader (== pixel shader) */
		{
			const FileReader reader{ "Resources/FragmentShader.txt" };

			/* Generate VertexShader ID */
			fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

			/* Attach the VertexShader code to the ID and compile it */
			const char* fragmentShaderSource{ reader.GetFileContents().c_str() };
			glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, nullptr);
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
		const uint32_t shaderProgramID{ glCreateProgram() };

		/* Link all previously created shaders to the shader program */
		glAttachShader(shaderProgramID, vertexShaderID);
		glAttachShader(shaderProgramID, fragmentShaderID);
		glLinkProgram(shaderProgramID);

		/* Check if the shader linking succeeded */
		int success{};
		char infoLog[512]{};
		glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(shaderProgramID, 512, nullptr, infoLog);
			Debug::LogError(std::string("Shader program linking failed: ") + infoLog, false);
		}

		/* Use our shader program! */
		glUseProgram(shaderProgramID);

		/* Delete our created shaders, they're no longer needed after linking */
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	Renderer& Renderer::GetInstance()
	{
		if (!Instance)
		{
			Instance = std::make_unique<Renderer>();
		}

		return *Instance.get();
	}

	void Renderer::Render()
	{
		const float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		/* Generate a vertex buffer ID */
		uint32_t vertexBufferID{};
		glGenBuffers(1, &vertexBufferID);

		/* Bind the ID to a vertex buffer */
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

		/* Copy our data into the buffer */
		/*
			GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
			GL_STATIC_DRAW: the data is set only once and used many times.
			GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
		*/
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	}
}