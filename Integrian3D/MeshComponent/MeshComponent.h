#pragma once

#include <stdint.h> /* uint32_t, ... */
#include <string_view> /* std::string_view */
#include <vector> /* std::vector */

namespace Integrian3D
{
	class MeshComponent final
	{
	public:
		explicit MeshComponent();
		explicit MeshComponent(const std::string& filePath);
		explicit MeshComponent(const std::vector<float>& vertices, const std::vector<uint32_t>& indices);
		~MeshComponent();

		MeshComponent(const MeshComponent&) noexcept = delete;
		MeshComponent(MeshComponent&& other) noexcept;
		MeshComponent& operator=(const MeshComponent&) noexcept = delete;
		MeshComponent& operator=(MeshComponent&& other) noexcept;

		uint32_t GetVertexArrayID() const { return VertexArrayID; }
		uint32_t GetIndexBufferID() const { return IndexBufferID; }
		uint32_t GetVertexBufferID() const { return VertexBufferID; }

		const std::vector<float> GetVertices() const { return Vertices; }
		const std::vector<uint32_t> GetIndices() const { return Indices; }

	private:
		uint32_t VertexArrayID;
		uint32_t VertexBufferID;
		uint32_t IndexBufferID;

		std::vector<float> Vertices;
		std::vector<uint32_t> Indices;
	};
}