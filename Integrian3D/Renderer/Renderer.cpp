#include "Renderer.h"

#include "DebugUtility/DebugUtility.h"
#include "Components/MeshComponent/MeshComponent.h"
#include "Components/TransformComponent/TransformComponent.h"
#include "Components/FreeCameraComponent/FreeCameraComponent.h"
#include "Math/GlmToRay.h"
#include "Texture/Texture.h"

#include <raylib.h>

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
		BeginDrawing();
		ClearBackground(Color{ 51, 77, 77, 1 });

		BeginMode3D(*camera.GetRayLibCamera());
	}

	void Renderer::Render(const MeshComponent& mesh, const TransformComponent& transform) const
	{
		DrawModelEx(*mesh.GetRayLibModel(), ToRay3(transform.GetLocalLocation()), ToRayRotation(transform.GetLocalAngle()), )
	}
}