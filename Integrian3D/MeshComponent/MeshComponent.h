#pragma once

#include <stdint.h> /* uint32_t, ... */
#include <string_view> /* std::string_view */

namespace Integrian3D
{
	class MeshComponent final
	{
	public:
		explicit MeshComponent();
		explicit MeshComponent(const std::string& filePath);

	private:
		uint32_t VertexArrayID;
		uint32_t VertexBufferID;
		uint32_t IndexBufferID;
	};
}