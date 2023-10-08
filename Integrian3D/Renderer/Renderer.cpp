#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "DebugUtility/DebugUtility.h"
#include "Components/MeshComponent/MeshComponent.h"
#include "Components/TransformComponent/TransformComponent.h"
#include "Components/FreeCameraComponent/FreeCameraComponent.h"
#include "Shader/Shader.h"
#include "Texture/Texture.h"
#include "IO/Ini/IniFile.h"

namespace Integrian3D
{
	Renderer::Renderer()
		: bShouldRenderWireframe{}
		, Shader{}
	{
		glEnable(GL_DEPTH_TEST);

		const IniFile EngineIni{ "Config/Engine.ini" };

		std::string VSPath{}, FSPath{};

		ICHECK_MSG(EngineIni.GetString("Renderer", "VertexShader", VSPath), "Renderer could not get Vertex Shader path from Engine.ini");
		ICHECK_MSG(EngineIni.GetString("Renderer", "FragmentShader", FSPath), "Renderer could not get Fragment Shader path from Engine.ini");

		Shader.SetShaders(VSPath, FSPath);
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

		// Set Light Position [TODO]: Dont hardcode this!
		Shader.SetVec3("_LightPos", Math::Vec3D{ 5.0, 0.0, 2.0 });

		// Set Light Direction [TODO]: Don't hardcode this
		Shader.SetVec3("_LightColor", Math::Vec3D{ 255.0, 0.0, 0.0 });

		/* Activate the texture */
		glActiveTexture(GL_TEXTURE0);
		
		/* Bind the Texture ID */
		glBindTexture(GL_TEXTURE_2D, mesh.GetTextures()[0]->GetTextureID());

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