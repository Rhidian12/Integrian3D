#pragma once 

#include "EngineConstants.h"

#include "Array/Array.h"
#include "Math/Math.h"
#include "Light/PointLight.h"
#include "TPair/TPair.h"
#include "Components/TransformComponent/TransformComponent.h"

#include <memory> /* std::unique_ptr */

namespace Integrian3D
{
	class FreeCameraComponent;
	class MeshComponent;
	class Light;

	class Renderer final
	{
	public:
		static Renderer& GetInstance();

		Renderer(const Renderer&) noexcept = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) noexcept = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void StartRenderLoop(const FreeCameraComponent& camera, const TransformComponent& Transform);

		// [CRINGE]: Just collect a pointer to a light base class
		void CollectLight(const TransformComponent& Transform, const PointLight& Light);

		void Render(const MeshComponent& mesh, const TransformComponent& transform) const;

		void ShouldRenderWireframe(const bool bShouldRenderWireFrame) { bShouldRenderWireframe = bShouldRenderWireFrame; }

	private:
		Renderer();

		friend std::unique_ptr<Renderer> std::make_unique();
		inline static std::unique_ptr<Renderer> Instance{};

		Math::Mat4D View;
		Math::Mat4D Projection;
		Math::Vec3D CameraPosition;

		// [CRINGE]: Change this to a pointer of a light base class
		TArray<TPair<TransformComponent, Light*>> Lights;

		bool bShouldRenderWireframe;
	};
}