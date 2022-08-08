#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../FileReader/FileReader.h"
#include "../DebugUtility/DebugUtility.h"
#include "../MeshComponent/MeshComponent.h"
#include "../Shader/Shader.h"

namespace Integrian3D
{
	Renderer::Renderer()
		: ShaderProgramID{}
		, bShouldRenderWireframe{}
	{
		Shader vertexShader{ "Resources/VertexShader.txt" }, fragmentShader{ "Resources/FragmentShader.txt" };

		/* Create shader program */
		ShaderProgramID = glCreateProgram();

		/* Link all previously created shaders to the shader program */
		glAttachShader(ShaderProgramID, vertexShader.GetShaderID());
		glAttachShader(ShaderProgramID, fragmentShader.GetShaderID());
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

		if (!bShouldRenderWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
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