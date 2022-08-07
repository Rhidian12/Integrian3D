#include "MeshComponent.h"

#include <glad/glad.h> /* OpenGL Dependency */
#include <assert.h> /* assert() */

#include "../FileReader/FileReader.h"

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
		/* Generate a vertex array ID */
		glGenVertexArrays(1, &VertexArrayID);

		/* Generate a vertex buffer ID */
		glGenBuffers(1, &VertexBufferID);

		/* Generate an Index Buffer ID */
		glGenBuffers(1, &IndexBufferID);

		/* Bind our Vertex Array */
		glBindVertexArray(VertexArrayID);

		/* Bind the ID to a vertex buffer */
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);

		/* Bind the ID to a index buffer */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);

		/* Copy our actual points into the buffer */
			/*
				GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
				GL_STATIC_DRAW: the data is set only once and used many times.
				GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
			*/
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

		/* Copy our actual indices into the buffer */
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

		/* Set Vertex Buffer Attribute Position layout */
		/*		1		  2			3     */
		/*  |X  Y  Z| |X  Y  Z| |X  Y  Z| */
		/*
		Position Attribute:
		Stride = 12
		Offset = 0
		*/
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));

		/* Enable the Position Attribute */
		glEnableVertexAttribArray(0);
	}

	MeshComponent::~MeshComponent()
	{
		glDeleteVertexArrays(1, &VertexArrayID);
		glDeleteBuffers(1, &VertexBufferID);
		glDeleteBuffers(1, &IndexBufferID);
	}
}