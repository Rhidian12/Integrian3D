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
		, Vertices{}
		, Indices{}
	{}

	MeshComponent::MeshComponent(const std::string& filePath)
		: VertexArrayID{}
		, VertexBufferID{}
		, IndexBufferID{}
		, Vertices{}
		, Indices{}
	{
		if (!filePath.empty())
		{
			assert(filePath.substr(filePath.find_last_of('.') + 1) == ".obj");

			FileReader reader{ filePath };

			const std::string fileContents{ reader.GetFileContents() };

			/* [TODO]: Parse .obj file */
		}
	}

	MeshComponent::MeshComponent(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
		: VertexArrayID{}
		, VertexBufferID{}
		, IndexBufferID{}
		, Vertices{ vertices }
		, Indices{ indices }
	{
		/* Generate a vertex array ID */
		glGenVertexArrays(1, &VertexArrayID);

		/* Generate a vertex buffer ID (VBO) */
		glGenBuffers(1, &VertexBufferID);

		/* Generate an Index Buffer ID (EBO) */
		glGenBuffers(1, &IndexBufferID);

		/* First bind the Vertex Array ID (VAO) */
		glBindVertexArray(VertexArrayID);

		/* Copy our actual points into the buffer */
			/*
				GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
				GL_STATIC_DRAW: the data is set only once and used many times.
				GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
			*/
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 3, vertices.data(), GL_STATIC_DRAW);

		/* Copy our actual indices into the buffer */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

		/* Enable the Position Attribute */
		glEnableVertexAttribArray(0);

		/* Set Vertex Buffer Attribute Position layout */
		/*		1		  2			3     */
		/*  |X  Y  Z| |X  Y  Z| |X  Y  Z| */
		/*
		Position Attribute:
		Stride = 12
		Offset = 0
		*/
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));

		glBindVertexArray(0);
	}

	MeshComponent::~MeshComponent()
	{
		if (VertexArrayID != std::numeric_limits<uint32_t>::max())
		{
			glDeleteVertexArrays(1, &VertexArrayID);
		}

		if (VertexBufferID != std::numeric_limits<uint32_t>::max())
		{
			glDeleteBuffers(1, &VertexBufferID);
		}

		if (IndexBufferID != std::numeric_limits<uint32_t>::max())
		{
			glDeleteBuffers(1, &IndexBufferID);
		}
	}

	MeshComponent::MeshComponent(MeshComponent&& other) noexcept
		: VertexArrayID{ std::move(other.VertexArrayID) }
		, VertexBufferID{ std::move(other.VertexBufferID) }
		, IndexBufferID{ std::move(other.IndexBufferID) }
		, Vertices{ std::move(other.Vertices) }
		, Indices{ std::move(other.Indices) }
	{
		other.Vertices.clear();
		other.Indices.clear();

		other.VertexArrayID = std::numeric_limits<uint32_t>::max();
		other.VertexBufferID = std::numeric_limits<uint32_t>::max();
		other.IndexBufferID = std::numeric_limits<uint32_t>::max();
	}

	MeshComponent& MeshComponent::operator=(MeshComponent&& other) noexcept
	{
		VertexArrayID = std::move(other.VertexArrayID);
		VertexBufferID = std::move(other.VertexBufferID);
		IndexBufferID = std::move(other.IndexBufferID);
		Vertices = std::move(other.Vertices);
		Indices = std::move(other.Indices);

		other.Vertices.clear();
		other.Indices.clear();

		other.VertexArrayID = std::numeric_limits<uint32_t>::max();
		other.VertexBufferID = std::numeric_limits<uint32_t>::max();
		other.IndexBufferID = std::numeric_limits<uint32_t>::max();

		return *this;
	}
}