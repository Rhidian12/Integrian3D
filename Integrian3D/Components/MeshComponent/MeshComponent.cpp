#include "MeshComponent.h"

#include "IO/File/File.h"
#include "Renderer/Renderer.h"
#include "Texture/Texture.h"

#include <glad/glad.h> /* OpenGL Dependency */
#include <sstream>

namespace Integrian3D
{
	static constexpr uint32 MeshErrorValue = std::numeric_limits<uint32>::max();

	namespace
	{
		void ParseObjFile(const std::string_view Filepath, TArray<Vertex>& OutVertices, TArray<uint32>& OutIndices)
		{
			const IO::File ObjFile{ Filepath, IO::OpenMode::OpenExisting, IO::FileMode::ASCII };

			const std::string_view Filecontents{ ObjFile.GetFileContents() };
			std::stringstream Stream{ Filecontents.data() };
			TArray<Math::Vec2D> UVs{};

			std::string Line{};
			while (std::getline(Stream, Line))
			{
				if (Line.empty())
				{
					continue;
				}

				switch (Line[0])
				{
					case '#':
						continue;
						break;
					case 'v':
					{
						if (Line[1] == 't')
						{
							std::stringstream LineStream{ Line.substr(2) };
							Math::Vec2D UV{};
							LineStream >> UV.x >> UV.y;
							UVs.Add(UV);
						}
						else
						{
							std::stringstream LineStream{ Line.substr(1) };
							Vertex Vertex{};
							LineStream >> Vertex.Position.x >> Vertex.Position.y >> Vertex.Position.z;
							OutVertices.Add(Vertex);
						}
					}
					break;
					case 'f':
					{
						std::stringstream LineStream{ Line.substr(1) };
						static constexpr uint32 NrOfFaces{ 3 };
						uint32 Faces[NrOfFaces]{};
						LineStream >> Faces[0] >> Faces[1] >> Faces[2];

						for (uint32 Index{}; Index < NrOfFaces; ++Index)
						{
							--Faces[Index];
						}

						OutIndices.AddRange(Faces, NrOfFaces);
					}
					break;
					default:
						break;
				}
			}

			for (int32 Index{}; Index < UVs.Size(); ++Index)
			{
				OutVertices[Index].UV = UVs[Index];
			}
		}
	}

	MeshComponent::MeshComponent(const std::string_view Filepath, Texture* const pTex)
		: VertexArrayID{ MeshErrorValue }
		, VertexBufferID{ MeshErrorValue }
		, IndexBufferID{ MeshErrorValue }
		, Vertices{}
		, Indices{}
		, pTexture{ pTex }
	{
		if (!Filepath.empty())
		{
			__ASSERT(Filepath.substr(Filepath.find_last_of('.')) == ".obj", "MeshComponent can only deserialize .obj files currently");

			ParseObjFile(Filepath, Vertices, Indices);

			SetupMesh();
		}
	}

	MeshComponent::MeshComponent(const TArray<Vertex>& vertices, const TArray<uint32>& indices, Texture* const pTex)
		: VertexArrayID{ MeshErrorValue }
		, VertexBufferID{ MeshErrorValue }
		, IndexBufferID{ MeshErrorValue }
		, Vertices{ vertices }
		, Indices{ indices }
		, pTexture{ pTex }
	{
		SetupMesh();
	}

	MeshComponent::~MeshComponent()
	{
		if (VertexArrayID != MeshErrorValue)
		{
			glDeleteVertexArrays(1, &VertexArrayID);
			VertexArrayID = MeshErrorValue;
		}

		if (VertexBufferID != MeshErrorValue)
		{
			glDeleteBuffers(1, &VertexBufferID);
			VertexBufferID = MeshErrorValue;
		}

		if (IndexBufferID != MeshErrorValue)
		{
			glDeleteBuffers(1, &IndexBufferID);
			IndexBufferID = MeshErrorValue;
		}
	}

	MeshComponent::MeshComponent(MeshComponent&& other) noexcept
		: VertexArrayID{ __MOVE(other.VertexArrayID) }
		, VertexBufferID{ __MOVE(other.VertexBufferID) }
		, IndexBufferID{ __MOVE(other.IndexBufferID) }
		, Vertices{ __MOVE(other.Vertices) }
		, Indices{ __MOVE(other.Indices) }
		, pTexture{ __MOVE(other.pTexture) }
	{
		other.Vertices.Clear();
		other.Indices.Clear();

		other.VertexArrayID = MeshErrorValue;
		other.VertexBufferID = MeshErrorValue;
		other.IndexBufferID = MeshErrorValue;

		other.pTexture = nullptr;
	}

	MeshComponent& MeshComponent::operator=(MeshComponent&& other) noexcept
	{
		VertexArrayID = __MOVE(other.VertexArrayID);
		VertexBufferID = __MOVE(other.VertexBufferID);
		IndexBufferID = __MOVE(other.IndexBufferID);
		Vertices = __MOVE(other.Vertices);
		Indices = __MOVE(other.Indices);
		pTexture = __MOVE(other.pTexture);

		other.Vertices.Clear();
		other.Indices.Clear();

		other.VertexArrayID = MeshErrorValue;
		other.VertexBufferID = MeshErrorValue;
		other.IndexBufferID = MeshErrorValue;

		other.pTexture = nullptr;

		return *this;
	}

	void MeshComponent::SetupMesh()
	{
		/* Generate a vertex array ID (VAO) */
		glGenVertexArrays(1, &VertexArrayID);

		/* Generate a vertex buffer ID (VBO) */
		glGenBuffers(1, &VertexBufferID);

		/* Generate an Index Buffer ID (EBO) */
		glGenBuffers(1, &IndexBufferID);

		/* First bind the Vertex Array ID (VAO) */
		glBindVertexArray(/* VAO = */ VertexArrayID);

		/* Copy our actual points into the buffer */
			/*
				GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
				GL_STATIC_DRAW: the data is set only once and used many times.
				GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
			*/
		glBindBuffer(GL_ARRAY_BUFFER, /* VBO = */ VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, Vertices.Size() * sizeof(Vertex), Vertices.Data(), GL_STATIC_DRAW);

		/* Copy our actual indices into the buffer */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, /* EBO = */ IndexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.Size() * sizeof(uint32_t), Indices.Data(), GL_STATIC_DRAW);

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

		glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(Vertex), /* offset = */ reinterpret_cast<void*>(0));

		/* Enable the UV Coord attribute */
		glEnableVertexAttribArray(1);

		/* Set Vertex Buffer UV Coord Attribute layout */
		glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, sizeof(Vertex), /* offset = */ reinterpret_cast<void*>(sizeof(Math::Vec3D)));

		glBindVertexArray(0);
	}
}