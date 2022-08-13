#pragma once 

#include "../EngineConstants.h"
#include "../Shader/Shader.h"
#include "../Components/CameraComponent/CameraComponent.h"

#include <memory> /* std::unique_ptr */

namespace Integrian3D
{
	class Renderer final
	{
	public:
		static Renderer& GetInstance();

		Renderer(const Renderer&) noexcept = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) noexcept = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void StartRenderLoop(const CameraComponent& camera) const;

		void Render(const class MeshComponent& meshComponent, const class TransformComponent& transformComponent) const;

		void ShouldRenderWireframe(const bool bShouldRenderWireFrame) { bShouldRenderWireframe = bShouldRenderWireFrame; }

		__NODISCARD const Shader& GetShader() const { return Shader; }

	private:
		Renderer();

		friend std::unique_ptr<Renderer> std::make_unique();
		inline static std::unique_ptr<Renderer> Instance{};

		uint32_t ShaderProgramID;
		bool bShouldRenderWireframe;
		Shader Shader;
	};
}