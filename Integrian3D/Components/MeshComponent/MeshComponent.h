#pragma once

#include "EngineConstants.h"

#include "Array/Array.h"
#include "Component/Component.h"
#include "Vertex/Vertex.h"

#include <string>

DEFINE_LOG_CATEGORY(LogMeshComponent, Integrian3D::LogVerbosity::Verbose);

namespace Integrian3D
{
	class Texture;

	class MeshComponent final : public Component
	{
	public:
		MeshComponent(const std::string_view Filepath, Texture* const pTex);
		MeshComponent(const TArray<Vertex>& vertices, const TArray<uint32>& indices, Texture* const pTex);
		~MeshComponent();

		MeshComponent(const MeshComponent&) noexcept = delete;
		MeshComponent(MeshComponent&& other) noexcept;
		MeshComponent& operator=(const MeshComponent&) noexcept = delete;
		MeshComponent& operator=(MeshComponent&& other) noexcept;

		__NODISCARD uint32 GetVertexArrayID() const { return VertexArrayID; }
		__NODISCARD uint32 GetIndexBufferID() const { return IndexBufferID; }
		__NODISCARD uint32 GetVertexBufferID() const { return VertexBufferID; }

		__NODISCARD const TArray<Vertex>& GetVertices() const { return Vertices; }
		__NODISCARD const TArray<uint32>& GetIndices() const { return Indices; }

		__NODISCARD const Texture* const GetTexture() const { return pTexture; }

	private:
		void SetupMesh();

		uint32 VertexArrayID;
		uint32 VertexBufferID;
		uint32 IndexBufferID;

		TArray<Vertex> Vertices;
		TArray<uint32> Indices;

		Texture* pTexture;
	};
}