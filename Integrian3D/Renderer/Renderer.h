#pragma once 

#include "../Shader/Shader.h"

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

		void StartRenderLoop();

		void Render(const class MeshComponent& meshComponent, const class TransformComponent& transformComponent);

		void ShouldRenderWireframe(const bool bShouldRenderWireFrame) { bShouldRenderWireframe = bShouldRenderWireFrame; }

		const Shader& GetShader() const { return Shader; }

	private:
		Renderer();

		friend std::unique_ptr<Renderer> std::make_unique();
		inline static std::unique_ptr<Renderer> Instance{};

		uint32_t ShaderProgramID;
		bool bShouldRenderWireframe;
		Shader Shader;
	};
}