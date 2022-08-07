#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../FileReader/FileReader.h"
#include "../DebugUtility/DebugUtility.h"
#include "../MeshComponent/MeshComponent.h"

namespace Integrian3D
{
	Renderer::Renderer()
		: ShaderProgramID{}
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
		{
			ShaderProgramID = glCreateProgram();

			/* Link all previously created shaders to the shader program */
			glAttachShader(ShaderProgramID, vertexShaderID);
			glAttachShader(ShaderProgramID, fragmentShaderID);
			glLinkProgram(ShaderProgramID);

			/* Check if the shader linking succeeded */
			int success{};
			char infoLog[512]{};
			glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &success);

			if (!success)
			{
				glGetProgramInfoLog(ShaderProgramID, 512, nullptr, infoLog);
				Debug::LogError(std::string("Shader program linking failed: ") + infoLog, false);
			}

			/* Delete our created shaders, they're no longer needed after linking */
			glDeleteShader(vertexShaderID);
			glDeleteShader(fragmentShaderID);
		}
	}

	Renderer::~Renderer()
	{
		glDeleteProgram(ShaderProgramID);
	}

	Renderer& Renderer::GetInstance()
	{
		if (!Instance)
		{
			Instance = std::make_unique<Renderer>();
		}

		return *Instance.get();
	}

	void Renderer::StartRenderLoop()
	{
		/* Sets the Clear Colour */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::Render(const MeshComponent& meshComponent)
	{
		/* Use our shader program! */
		glUseProgram(ShaderProgramID);

		/* Bind the Vertex Array ID */
		glBindVertexArray(meshComponent.GetVertexArrayID());

		/* Bind the ID to a vertex buffer */
		glBindBuffer(GL_ARRAY_BUFFER, meshComponent.GetVertexBufferID());

		/* Bind the ID to an index buffer */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshComponent.GetIndexBufferID());

		/* Render our rectangle */
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(meshComponent.GetIndices().size()), GL_UNSIGNED_INT, 0);
	}
}