#pragma once 

#include "EngineConstants.h"

#include "Array/Array.h"
#include "Math/Math.h"
#include "TPair/TPair.h"
#include "Components/TransformComponent/TransformComponent.h"

#include <memory> /* std::unique_ptr */

namespace Integrian3D
{
	class DirectionalLight;
	class FreeCameraComponent;
	class Light;
	class MeshComponent;
	class PointLight;

	class Renderer final
	{
	public:
		static Renderer& GetInstance();

		Renderer(const Renderer&) noexcept = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) noexcept = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void StartRenderLoop(const FreeCameraComponent& camera, const TransformComponent& Transform);

		void CollectPointLight(const TransformComponent& Transform, const PointLight& Light);
		void CollectDirectionalLight(const TransformComponent& Transform, const DirectionalLight& DLight);

		void Render(MeshComponent& mesh, const TransformComponent& transform) const;

		void ShouldRenderWireframe(const bool bShouldRenderWireFrame) { bShouldRenderWireframe = bShouldRenderWireFrame; }

	private:
		Renderer();

		friend std::unique_ptr<Renderer> std::make_unique();
		inline static std::unique_ptr<Renderer> Instance{};

		Math::Mat4D View;
		Math::Mat4D Projection;
		Math::Vec3D CameraPosition;

		TArray<TPair<TransformComponent, Light*>> Lights;
		TArray<Light*> DirectionalLights;

		bool bShouldRenderWireframe;
	};
}