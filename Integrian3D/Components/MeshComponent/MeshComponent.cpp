#include "MeshComponent.h"

#include "../../FileReader/FileReader.h"

#include <sstream> /* std::sstream */
#include <array> /* std::array */
#include <glad/glad.h> /* OpenGL Dependency */
#include <regex>

namespace Integrian3D
{
	struct Face final
	{
		constexpr static size_t NrOfVertices{ 3 };

		std::array<int, NrOfVertices> vertexIndices;
		std::array<int, NrOfVertices> uvIndices;
		std::array<int, NrOfVertices> normalIndices;
	};

	MeshComponent::MeshComponent(const std::string_view filePath, Texture* const pTex)
		: VertexArrayID{}
		, VertexBufferID{}
		, IndexBufferID{}
		, Vertices{}
		, Indices{}
		, pTexture{ pTex }
	{
		if (!filePath.empty())
		{
			__ASSERT(filePath.substr(filePath.find_last_of('.')) == ".obj");

			FileReader reader{ filePath.data() };

			const std::string fileContents{ reader.GetFileContents() };

			ParseOBJFile(fileContents);

			InitializeOpenGL();
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
		InitializeOpenGL();
	}

	MeshComponent::~MeshComponent()
	{
		if (VertexArrayID != std::numeric_limits<uint32_t>::max())
			glDeleteVertexArrays(1, &VertexArrayID);

		if (VertexBufferID != std::numeric_limits<uint32_t>::max())
			glDeleteBuffers(1, &VertexBufferID);

		if (IndexBufferID != std::numeric_limits<uint32_t>::max())
			glDeleteBuffers(1, &IndexBufferID);
	}

	MeshComponent::MeshComponent(MeshComponent&& other) noexcept
		: VertexArrayID{ __MOVE(uint32_t, other.VertexArrayID) }
		, VertexBufferID{ __MOVE(uint32_t, other.VertexBufferID) }
		, IndexBufferID{ __MOVE(uint32_t, other.IndexBufferID) }
		, Vertices{ __MOVE(std::vector<Vertex>, other.Vertices) }
		, Indices{ __MOVE(std::vector<uint32_t>, other.Indices) }
		, pTexture{ __MOVE(Texture*, other.pTexture) }
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
		VertexArrayID = __MOVE(uint32_t, other.VertexArrayID);
		VertexBufferID = __MOVE(uint32_t, other.VertexBufferID);
		IndexBufferID = __MOVE(uint32_t, other.IndexBufferID);
		Vertices = __MOVE(std::vector<Vertex>, other.Vertices);
		Indices = __MOVE(std::vector<uint32_t>, other.Indices);
		pTexture = __MOVE(Texture*, other.pTexture);

		other.Vertices.clear();
		other.Indices.clear();

		other.VertexArrayID = std::numeric_limits<uint32_t>::max();
		other.VertexBufferID = std::numeric_limits<uint32_t>::max();
		other.IndexBufferID = std::numeric_limits<uint32_t>::max();

		other.pTexture = nullptr;

		return *this;
	}

	void MeshComponent::InitializeOpenGL()
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
		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), Vertices.data(), GL_STATIC_DRAW);

		/* Copy our actual indices into the buffer */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(uint32_t), Indices.data(), GL_STATIC_DRAW);

		/* Set Vertex Buffer Position Attribute layout */
		/*		   1			   2			   3		*/
		/*  |X  Y  Z  U  V| |X  Y  Z  U  V| |X  Y  Z  U  V|	*/
		/*
		Position Attribute:
		Stride = 20
		Offset = 0
		UV Attribute:
		Stride = 20
		Offset = 12
		*/

		/* Enable the Position Attribute */
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));

		/* Enable the UV Coord attribute */
		glEnableVertexAttribArray(1);

		/* Set Vertex Buffer UV Coord Attribute layout */
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(glm::vec3)));

		glBindVertexArray(0);
	}

	void MeshComponent::ParseOBJFile(const std::string& fileContents)
	{
		using namespace Math;

		std::stringstream ss{ fileContents };
		std::string line{};

		std::vector<Vec3D> vertices{};
		std::vector<Vec2D> uvs{};
		std::vector<Face> faces{};

		while (!std::getline(ss, line).eof())
		{
			if (line[0] == '#')
				continue;
			else if (line[0] == 'v' && line[1] == 't')
			{
				std::stringstream data{ line.substr(line.find_first_not_of('t')) };

				Vec2D v{};
				data >> v.x >> v.y;

				uvs.push_back(v);
			}
			/* [TODO[: add Vertex normals */
			else if (line[0] == 'v')
			{
				std::stringstream data{ line.substr(line.find_first_not_of('v')) };

				Vec3D v{};
				data >> v.x >> v.y >> v.z;

				vertices.push_back(v);
			}
			else if (line[0] == 'f')
			{
				std::stringstream data{ line.substr(line.find_first_not_of('f')) };

				int vIndex1{}, vIndex2{}, vIndex3{};
				int uvIndex1{}, uvIndex2{}, uvIndex3{};
				int normalIndex1{}, normalIndex2{}, normalIndex3{};
				std::smatch matches{};
				std::regex regexExpression{ "f (\\d+)\\/(\\d+)\\/(\\d+) (\\d+)\\/(\\d+)\\/(\\d+) (\\d+)\\/(\\d+)\\/(\\d+)" };

				Face face{};

				std::regex_search(line, matches, regexExpression);

				vIndex1 = std::stoi(matches[1]);
				vIndex2 = std::stoi(matches[4]);
				vIndex3 = std::stoi(matches[7]);

				face.vertexIndices[0] = --vIndex1;
				face.vertexIndices[1] = --vIndex2;
				face.vertexIndices[2] = --vIndex3;

				uvIndex1 = std::stoi(matches[2]);
				uvIndex2 = std::stoi(matches[5]);
				uvIndex3 = std::stoi(matches[8]);

				face.uvIndices[0] = --uvIndex1;
				face.uvIndices[1] = --uvIndex2;
				face.uvIndices[2] = --uvIndex3;

				normalIndex1 = std::stoi(matches[3]);
				normalIndex2 = std::stoi(matches[6]);
				normalIndex3 = std::stoi(matches[9]);

				face.normalIndices[0] = --normalIndex1;
				face.normalIndices[1] = --normalIndex2;
				face.normalIndices[2] = --normalIndex3;

				faces.push_back(face);
			}
		}

		for (const Face& face : faces)
		{
			for (size_t i{}; i < Face::NrOfVertices; ++i)
			{
				int uvIndex{ face.uvIndices[i] };
				int vertexIndex{ face.vertexIndices[i] };
				// int normalIndex{ face.normalIndices[i] - 1 };

				const Vec2D uv{ uvs[uvIndex] };
				const Vec3D position{ vertices[vertexIndex] };
				// const Vec3D normal{ normals[normalIndex] };

				Vertices.push_back(Vertex{ position,uv });
				Indices.push_back(static_cast<uint32_t>(Vertices.size() - 1));
			}
		}
	}
}