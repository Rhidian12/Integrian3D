#pragma once 

#include "EngineConstants.h"

#include "Math/Math.h"

#include <memory> /* std::unique_ptr */

namespace Integrian3D
{
	class FreeCameraComponent;
	class MeshComponent;
	class TransformComponent;

	class Renderer final
	{
	public:
		static Renderer& GetInstance();

		Renderer(const Renderer&) noexcept = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) noexcept = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void StartRenderLoop(const FreeCameraComponent& camera);

		void Render(const MeshComponent& mesh, const TransformComponent& transform) const;

		void ShouldRenderWireframe(const bool bShouldRenderWireFrame) { bShouldRenderWireframe = bShouldRenderWireFrame; }

	private:
		Renderer();

		friend std::unique_ptr<Renderer> std::make_unique();
		inline static std::unique_ptr<Renderer> Instance{};

		Math::Mat4D View;
		Math::Mat4D Projection;

		bool bShouldRenderWireframe;
	};
}