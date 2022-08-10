#pragma once

#include "../Vertex/Vertex.h"
#include "../Texture/Texture.h"

#include <stdint.h> /* uint32_t, ... */
#include <string_view> /* std::string_view */
#include <vector> /* std::vector */

namespace Integrian3D
{
	class MeshComponent final
	{
	public:
		MeshComponent() = default;
		MeshComponent(const std::string& filePath, Texture* const pTex);
		MeshComponent(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, Texture* const pTex);
		~MeshComponent();

		MeshComponent(const MeshComponent&) noexcept = delete;
		MeshComponent(MeshComponent&& other) noexcept;
		MeshComponent& operator=(const MeshComponent&) noexcept = delete;
		MeshComponent& operator=(MeshComponent&& other) noexcept;

		uint32_t GetVertexArrayID() const { return VertexArrayID; }
		uint32_t GetIndexBufferID() const { return IndexBufferID; }
		uint32_t GetVertexBufferID() const { return VertexBufferID; }

		const std::vector<Vertex> GetVertices() const { return Vertices; }
		const std::vector<uint32_t> GetIndices() const { return Indices; }

		const Texture* const GetTexture() const { return pTexture; }

	private:
		uint32_t VertexArrayID;
		uint32_t VertexBufferID;
		uint32_t IndexBufferID;

		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;

		Texture* pTexture;
	};
}