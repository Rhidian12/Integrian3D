#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../FileReader/FileReader.h"
#include "../DebugUtility/DebugUtility.h"

namespace Integrian3D
{
	Renderer::Renderer()
		: ShaderProgramID{}
		, VertexArrayID{}
		, VertexBufferID{}
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

		const float vertices[] =
		{
			 0.5f,  0.5f, 0.0f,  // top right
			 0.5f, -0.5f, 0.0f,  // bottom right
			-0.5f, -0.5f, 0.0f,  // bottom left
			-0.5f,  0.5f, 0.0f   // top left 
		};

		const uint32_t indices[] =
		{
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};

		/* Generate a vertex array ID */
		glGenVertexArrays(1, &VertexArrayID);

		/* Generate a vertex buffer ID */
		glGenBuffers(1, &VertexBufferID);

		/* Generate an Index Buffer ID */
		glGenBuffers(1, &IndexBufferID);

		/* Bind our Vertex Array */
		glBindVertexArray(VertexArrayID);

		/* Bind the ID to a vertex buffer */
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);

		/* Bind the ID to a index buffer */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);

		/* Copy our actual points into the buffer */
			/*
				GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
				GL_STATIC_DRAW: the data is set only once and used many times.
				GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
			*/
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		/* Copy our actual indices into the buffer */
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		/* Set Vertex Buffer Attribute Position layout */
		/*		1		  2			3     */
		/*  |X  Y  Z| |X  Y  Z| |X  Y  Z| */
		/*
		Position Attribute:
		Stride = 12
		Offset = 0
		*/
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));

		/* Enable the Position Attribute */
		glEnableVertexAttribArray(0);
	}

	Renderer::~Renderer()
	{
		glDeleteVertexArrays(1, &VertexArrayID);
		glDeleteBuffers(1, &VertexBufferID);
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

	void Renderer::Render()
	{
		/* Sets the Clear Colour */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/* Use our shader program! */
		glUseProgram(ShaderProgramID);

		/* Bind the Vertex Array ID */
		glBindVertexArray(VertexArrayID);

		/* Render our rectangle */
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}