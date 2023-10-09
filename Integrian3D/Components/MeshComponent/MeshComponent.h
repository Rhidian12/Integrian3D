#pragma once

#include "EngineConstants.h"

#include "Array/Array.h"
#include "Component/Component.h"
#include "Math/Math.h"
#include "Memory/UniquePtr.h"
#include "Vertex/Vertex.h"

#include <string>

DEFINE_LOG_CATEGORY(LogMeshComponent, Integrian3D::LogVerbosity::Verbose);

namespace Integrian3D
{
	class Texture;
	class Material;

	class MeshComponent final : public Component
	{
	public:
		MeshComponent(const std::string_view Filepath, UniquePtr<Material>&& Material);
		MeshComponent(const TArray<Vertex>& vertices, const TArray<uint32>& indices, UniquePtr<Material>&& Material);
		~MeshComponent();

		MeshComponent(const MeshComponent&) noexcept = delete;
		MeshComponent(MeshComponent&& other) noexcept;
		MeshComponent& operator=(const MeshComponent&) noexcept = delete;
		MeshComponent& operator=(MeshComponent&& other) noexcept;

		void AddTexture(Texture* const TextureToApply);

		void StartShader(const Math::Mat4D& Transform, const Math::Mat4D& View, const Math::Mat4D& Projection) const;

		__NODISCARD uint32 GetVertexArrayID() const { return VertexArrayID; }
		__NODISCARD uint32 GetIndexBufferID() const { return IndexBufferID; }
		__NODISCARD uint32 GetVertexBufferID() const { return VertexBufferID; }

		__NODISCARD const TArray<Vertex>& GetVertices() const { return Vertices; }
		__NODISCARD const TArray<uint32>& GetIndices() const { return Indices; }

	private:
		void SetupMesh();

		uint32 VertexArrayID;
		uint32 VertexBufferID;
		uint32 IndexBufferID;

		UniquePtr<Material> MeshMaterial;

		TArray<Vertex> Vertices;
		TArray<uint32> Indices;
		TArray<Texture*> Textures;
	};
}