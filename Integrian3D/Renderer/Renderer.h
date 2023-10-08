#pragma once 

#include "EngineConstants.h"

#include "Shader/Shader.h"

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

		void StartRenderLoop(const FreeCameraComponent& camera) const;

		void Render(const MeshComponent& mesh, const TransformComponent& transform) const;

		void ShouldRenderWireframe(const bool bShouldRenderWireFrame) { bShouldRenderWireframe = bShouldRenderWireFrame; }

		__NODISCARD const Shader& GetShader() const { return Shader; }

	private:
		Renderer();

		friend std::unique_ptr<Renderer> std::make_unique();
		inline static std::unique_ptr<Renderer> Instance{};

		bool bShouldRenderWireframe;
		Shader Shader;
	};
}