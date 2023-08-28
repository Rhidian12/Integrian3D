#pragma once

#include "../../EngineConstants.h"

#include "../../Array/Array.h"
#include "../../Component/Component.h"
#include "../../Vertex/Vertex.h"

#include <string>

namespace Integrian3D
{
	class Texture;

	class MeshComponent final : public Component
	{
	public:
		MeshComponent(const std::string& filePath, Texture* const pTex);
		MeshComponent(const TArray<Vertex>& vertices, const TArray<uint32_t>& indices, Texture* const pTex);
		~MeshComponent();

		MeshComponent(const MeshComponent&) noexcept = delete;
		MeshComponent(MeshComponent&& other) noexcept;
		MeshComponent& operator=(const MeshComponent&) noexcept = delete;
		MeshComponent& operator=(MeshComponent&& other) noexcept;

		__NODISCARD uint32_t GetVertexArrayID() const { return VertexArrayID; }
		__NODISCARD uint32_t GetIndexBufferID() const { return IndexBufferID; }
		__NODISCARD uint32_t GetVertexBufferID() const { return VertexBufferID; }

		__NODISCARD const TArray<Vertex>& GetVertices() const { return Vertices; }
		__NODISCARD const TArray<uint32_t>& GetIndices() const { return Indices; }

		__NODISCARD const Texture* const GetTexture() const { return pTexture; }

	private:
		uint32_t VertexArrayID;
		uint32_t VertexBufferID;
		uint32_t IndexBufferID;

		TArray<Vertex> Vertices;
		TArray<uint32_t> Indices;

		Texture* pTexture;
	};
}