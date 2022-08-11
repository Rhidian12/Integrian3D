#include "MeshComponent.h"

#include <glad/glad.h> /* OpenGL Dependency */
#include <assert.h> /* assert() */

#include "../../FileReader/FileReader.h"

namespace Integrian3D
{
	MeshComponent::MeshComponent(const std::string& filePath, Texture* const pTex)
		: VertexArrayID{}
		, VertexBufferID{}
		, IndexBufferID{}
		, Vertices{}
		, Indices{}
		, pTexture{ pTex }
	{
		if (!filePath.empty())
		{
			assert(filePath.substr(filePath.find_last_of('.') + 1) == ".obj");

			FileReader reader{ filePath };

			const std::string fileContents{ reader.GetFileContents() };

			/* [TODO]: Parse .obj file */
		}
	}

	MeshComponent::MeshComponent(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, Texture* const pTex)
		: VertexArrayID{}
		, VertexBufferID{}
		, IndexBufferID{}
		, Vertices{ vertices }
		, Indices{ indices }
		, pTexture{ pTex }
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
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		/* Copy our actual indices into the buffer */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

		/* Enable the Position Attribute */
		glEnableVertexAttribArray(0);

		/* Set Vertex Buffer Position Attribute layout */
		/*			   1							 2							   3					*/
		/*  |X  Y  Z  (R  G  B  A)  U  V| |X  Y  Z  (R  G  B  A)  U  V| |X  Y  Z  (R  G  B  A)  U  V|	*/
		/*
		Position Attribute:
		Stride = (36) 20
		Offset = 0
		Colour Attribute:
		Stride = (36) 20
		Offset = (12) 0
		UV Attribute:
		Stride = (36) 20
		Offset = (28) 12
		*/
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));

		/* Enable the Colour Attribute */
		glEnableVertexAttribArray(1);

		/* Set Vertex Buffer Colour Attribute layout */
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(Vector3f)));

		/* Enable the UV Coord attribute */
		glEnableVertexAttribArray(2);

		/* Set Vertex Buffer UV Coord Attribute layout */
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(Vertex) - sizeof(Point2f)));

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
		, pTexture{ std::move(other.pTexture) }
	{
		other.Vertices.clear();
		other.Indices.clear();

		other.VertexArrayID = std::numeric_limits<uint32_t>::max();
		other.VertexBufferID = std::numeric_limits<uint32_t>::max();
		other.IndexBufferID = std::numeric_limits<uint32_t>::max();

		other.pTexture = nullptr;
	}

	MeshComponent& MeshComponent::operator=(MeshComponent&& other) noexcept
	{
		VertexArrayID = std::move(other.VertexArrayID);
		VertexBufferID = std::move(other.VertexBufferID);
		IndexBufferID = std::move(other.IndexBufferID);
		Vertices = std::move(other.Vertices);
		Indices = std::move(other.Indices);
		pTexture = std::move(other.pTexture);

		other.Vertices.clear();
		other.Indices.clear();

		other.VertexArrayID = std::numeric_limits<uint32_t>::max();
		other.VertexBufferID = std::numeric_limits<uint32_t>::max();
		other.IndexBufferID = std::numeric_limits<uint32_t>::max();

		other.pTexture = nullptr;

		return *this;
	}
}