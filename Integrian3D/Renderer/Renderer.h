#pragma once 

#include <memory> /* std::unique_ptr */

namespace Integrian3D
{
	class Renderer final
	{
	public:
		static Renderer& GetInstance();

		~Renderer();

		Renderer(const Renderer&) noexcept = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) noexcept = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void StartRenderLoop();

		void Render(const class MeshComponent& meshComponent);

	private:
		Renderer();

		friend std::unique_ptr<Renderer> std::make_unique();
		inline static std::unique_ptr<Renderer> Instance{};

		uint32_t ShaderProgramID;
	};
}