#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Components/MeshComponent/MeshComponent.h"
#include "Components/TransformComponent/TransformComponent.h"
#include "Components/FreeCameraComponent/FreeCameraComponent.h"

#include "IO/Ini/IniFile.h"

#include "Light/PointLight.h"
#include "Light/DirectionalLight.h"

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

	void Renderer::CollectPointLight(const TransformComponent& Transform, const PointLight& PLight)
	{
		Lights.Add(MakePair(Transform, static_cast<Light*>(const_cast<PointLight*>(&PLight))));
	}

	void Renderer::CollectDirectionalLight(const TransformComponent& Transform, const DirectionalLight& DLight)
	{
		Lights.Add(MakePair(Transform, static_cast<Light*>(const_cast<DirectionalLight*>(&DLight))));
	}

	void Renderer::Render(MeshComponent& mesh, const TransformComponent& transform) const
	{
		mesh.Render(transform.Transformation, View, Projection, CameraPosition, Lights);
	}
}