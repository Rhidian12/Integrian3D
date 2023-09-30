#pragma once

#include "EngineConstants.h"

#include "Array/Array.h"
#include "Component/Component.h"

#include <string_view>

DEFINE_LOG_CATEGORY(LogMeshComponent, Integrian3D::LogVerbosity::Verbose);

struct Model;
namespace Integrian3D
{
	class Texture;

	class MeshComponent final : public Component
	{
	public:
		MeshComponent(const std::string_view Filepath, Texture* const pTex);
		~MeshComponent();

		MeshComponent(const MeshComponent&) noexcept = delete;
		MeshComponent(MeshComponent&& other) noexcept;
		MeshComponent& operator=(const MeshComponent&) noexcept = delete;
		MeshComponent& operator=(MeshComponent&& other) noexcept;

		__NODISCARD const Texture* const GetTexture() const { return pTexture; }
		__NODISCARD const Model* const GetRayLibModel() const;

	private:
		Model* ModelImplementation; // By Exception, this is owned by MeshComponent
		Texture* pTexture;
	};
}