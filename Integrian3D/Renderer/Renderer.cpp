#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../DebugUtility/DebugUtility.h"
#include "../Components/MeshComponent/MeshComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/FreeCameraComponent/FreeCameraComponent.h"
#include "../Shader/Shader.h"
#include "../Texture/Texture.h"

namespace Integrian3D
{
	Renderer::Renderer()
		: ShaderProgramID{}
		, bShouldRenderWireframe{}
		, Shader{ "Resources/VertexShader.txt", "Resources/FragmentShader.txt" }
	{
		glEnable(GL_DEPTH_TEST);
	}

	Renderer& Renderer::GetInstance()
	{
		if (!Instance)
		{
			Instance = std::make_unique<Renderer>();
		}

		return *Instance.get();
	}

	void Renderer::StartRenderLoop(const FreeCameraComponent& camera) const
	{
		/* Sets the Clear Colour */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (!bShouldRenderWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		/* Use our shader program! */
		Shader.Activate();

		Shader.SetMatrix("_View", camera.GetView());

		Shader.SetMatrix("_Projection", camera.GetProjection());
	}

	void Renderer::Render(const MeshComponent& mesh, const TransformComponent& transform) const
	{
		/* Set our Matrix */
		Shader.SetMatrix("_Transform", transform.Transformation);

		/* Activate the texture */
		glActiveTexture(GL_TEXTURE0);

		/* Bind the Texture ID */
		// glBindTexture(GL_TEXTURE_2D, mesh.GetTexture()->GetTextureID());

		/* Bind the Vertex Array ID */
		glBindVertexArray(mesh.GetVertexArrayID());

		/* Bind the ID to a vertex buffer */
		glBindBuffer(GL_ARRAY_BUFFER, mesh.GetVertexBufferID());

		/* Bind the ID to an index buffer */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.GetIndexBufferID());

		/* Render our rectangle */
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.GetIndices().Size()), GL_UNSIGNED_INT, 0);
	}
}