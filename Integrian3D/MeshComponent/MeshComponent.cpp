#include "MeshComponent.h"

#include "../FileReader/FileReader.h"

#include <assert.h> /* assert() */

namespace Integrian3D
{
	MeshComponent::MeshComponent()
		: VertexArrayID{}
		, VertexBufferID{}
		, IndexBufferID{}
	{}

	MeshComponent::MeshComponent(const std::string& filePath)
		: VertexArrayID{}
		, VertexBufferID{}
		, IndexBufferID{}
	{
		if (!filePath.empty())
		{
			assert(filePath.substr(filePath.find_last_of('.') + 1) == ".obj");

			FileReader reader{ filePath };

			const std::string fileContents{ reader.GetFileContents() };

			/* [TODO]: Parse .obj file */
		}
	}

	MeshComponent::MeshComponent(const std::vector<float>& vertices, const std::vector<uint32_t>& indices)
		: VertexArrayID{}
		, VertexBufferID{}
		, IndexBufferID{}
	{
	}
}