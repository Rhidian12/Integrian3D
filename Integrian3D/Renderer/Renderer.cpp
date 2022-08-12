#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../FileReader/FileReader.h"
#include "../DebugUtility/DebugUtility.h"
#include "../Components/MeshComponent/MeshComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Shader/Shader.h"

namespace Integrian3D
{
	Renderer::Renderer()
		: ShaderProgramID{}
		, bShouldRenderWireframe{}
		, Shader{ "Resources/VertexShader.txt", "Resources/FragmentShader.txt" }
	{}

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

	void Renderer::Render(const MeshComponent& meshComponent, const TransformComponent& transformComponent)
	{
		/* Use our shader program! */
		Shader.Activate();

		/* Set our Matrix */
		Shader.SetMatrix("_Transform", transformComponent.Transformation);

		/* Activate the texture */
		glActiveTexture(GL_TEXTURE0);

		/* Bind the Texture ID */
		glBindTexture(GL_TEXTURE_2D, meshComponent.GetTexture()->GetTextureID());

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