#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Components/MeshComponent/MeshComponent.h"
#include "Components/TransformComponent/TransformComponent.h"
#include "Components/FreeCameraComponent/FreeCameraComponent.h"
#include "IO/Ini/IniFile.h"
#include "Light/Light.h"
#include "Material/Material.h"
#include "Shader/Shader.h"
#include "Texture/Texture.h"

namespace Integrian3D
{
	Renderer::Renderer()
		: bShouldRenderWireframe{}
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

	void Renderer::StartRenderLoop(const FreeCameraComponent& camera, const TransformComponent& Transform)
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

		View = camera.GetView();
		Projection = camera.GetProjection();
		CameraPosition = Transform.GetLocalLocation();

		Lights.Clear();
	}

	void Renderer::CollectLight(const TransformComponent& Transform, const PointLight& PLight)
	{
		Lights.Add(MakePair(Transform, static_cast<Light*>(const_cast<PointLight*>(&PLight))));
	}

	void Renderer::Render(const MeshComponent& mesh, const TransformComponent& transform) const
	{
		mesh.GetMaterial()->StartShader(transform.Transformation, View, Projection, CameraPosition, Lights);

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