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

	private:
		uint32_t VertexArrayID;
		uint32_t VertexBufferID;
		uint32_t IndexBufferID;
	};
}