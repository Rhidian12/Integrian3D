#pragma once

#include <stdint.h>

namespace Integrian3D
{
	class MeshComponent final
	{
	public:
		explicit MeshComponent();

	private:
		uint32_t VertexArrayID;
		uint32_t VertexBufferID;
		uint32_t IndexBufferID;
	};
}