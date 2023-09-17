#include "MeshComponent.h"

#include "IO/File/File.h"
#include "Renderer/Renderer.h"
#include "Texture/Texture.h"

#include <glad/glad.h>
#include <sstream>

namespace Integrian3D
{
	static constexpr uint32 MeshErrorValue = std::numeric_limits<uint32>::max();

	namespace
	{
		static void SplitString(const std::string& OriginalString, const char SplitCharacter, TArray<std::string>& SplitParts)
		{
			SIZE_T Start{};
			SIZE_T SplitCharacterPos{ OriginalString.find(SplitCharacter) };

			while (SplitCharacterPos != std::string::npos)
			{
				SplitParts.Add(OriginalString.substr(Start, SplitCharacterPos - Start));

				Start = SplitCharacterPos + 1;
				SplitCharacterPos = OriginalString.find(SplitCharacter, Start);
			}

			if (Start < OriginalString.size())
			{
				SplitParts.Add(OriginalString.substr(Start));
			}
		}

		static void ParseObjFile(const std::string_view Filepath, TArray<Vertex>& OutVertices, TArray<uint32>& OutIndices)
		{
			const IO::File ObjFile{ Filepath, IO::OpenMode::OpenExisting, IO::FileMode::ASCII };

			const std::string_view Filecontents{ ObjFile.GetFileContents() };
			std::stringstream Stream{ Filecontents.data() };

			TArray<Math::Vec3D> Positions{};
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
							std::stringstream LineStream{ Line.substr(3) };
							Math::Vec2D UV{};
							LineStream >> UV.x >> UV.y;
							UVs.Add(UV);
						}
						else
						{
							std::stringstream LineStream{ Line.substr(2) };
							Math::Vec3D Position{};
							LineStream >> Position.x >> Position.y >> Position.z;
							Positions.Add(Position);
						}
					}
					break;
					case 'f':
					{
						TArray<std::string> FaceElements{};
						SplitString(Line.substr(2), ' ', FaceElements);

						FaceElements.Erase([](const std::string& Element)->bool
							{
								for (const char Character : Element)
								{
									if (std::isspace(Character))
									{
										return true;
									}
								}

								return false;
							});

						TArray<Vertex> Vertices{};
						TArray<uint32> Indices{};

						for (const std::string& FaceInfo : FaceElements)
						{
							Vertex Vertex{};
							const int64 NrOfElements{ std::count(FaceInfo.cbegin(), FaceInfo.cend(), '/') };

							switch (NrOfElements)
							{
								case 0: // Only index info
									Vertex.Position = Positions[std::stoi(FaceInfo) - 1];
									break;
								case 1: // Index + UV info
								{
									TArray<std::string> IndexUVInfo{};
									SplitString(FaceInfo, '/', IndexUVInfo);

									__ASSERT(IndexUVInfo.Size() == 2, "SplitString() is broken");

									Vertex.Position = Positions[std::stoi(IndexUVInfo[0]) - 1];
									Vertex.UV = UVs[std::stoi(IndexUVInfo[1]) - 1];
								}
								break;
								case 2: // Index + UV + Normal info
								{
									TArray<std::string> IndexUVInfo{};
									SplitString(FaceInfo, '/', IndexUVInfo);

									__ASSERT(IndexUVInfo.Size() == 3, "SplitString() is broken");

									Vertex.Position = Positions[std::stoi(IndexUVInfo[0]) - 1];
									Vertex.UV = UVs[std::stoi(IndexUVInfo[1]) - 1];

									// [TODO]: Handle Normals
								}
								break;
								default:
									__ASSERT(false, "");
									break;
							}

							Vertices.Add(Vertex);
						}

						const SIZE_T NrOfVertices{ Vertices.Size() };
						if (NrOfVertices < 3)
						{
							break;
						}
						else if (NrOfVertices == 3)
						{
							Indices.AddRange({ 0,1,2 });
						}
						else
						{
							TArray<Vertex> TempVertices{ Vertices };

							while (true)
							{
								for (int32 I{}; I < static_cast<int32>(TempVertices.Size()); ++I)
								{
									Vertex PreviousVertex{};
									if (I == 0)
									{
										PreviousVertex = TempVertices[TempVertices.Size() - 1];
									}
									else
									{
										PreviousVertex = TempVertices[I - 1];
									}

									Vertex CurrentVertex{ TempVertices[I] };

									Vertex NextVertex{};
									if (I == TempVertices.Size() - 1)
									{
										NextVertex = TempVertices[0];
									}
									else
									{
										NextVertex = TempVertices[I + 1];
									}

									// If there are only 3 vertices left, then we're on the last triangle
									if (TempVertices.Size() == 3)
									{
										// create a triangle from Previous, Current and Next
										for (uint32 J{}; J < static_cast<uint32>(TempVertices.Size()); ++J)
										{
											if (Vertices[J].Position == CurrentVertex.Position)
											{
												Indices.Add(J);
											}
											else if (Vertices[J].Position == PreviousVertex.Position)
											{
												Indices.Add(J);
											}
											else if (Vertices[J].Position == NextVertex.Position)
											{
												Indices.Add(J);
											}
										}

										TempVertices.Clear();
										break;
									}
									else if (TempVertices.Size() == 4)
									{
										// create a triangle from Previous, Current and Next
										for (uint32 J{}; J < static_cast<uint32>(TempVertices.Size()); ++J)
										{
											if (Vertices[J].Position == CurrentVertex.Position)
											{
												Indices.Add(J);
											}
											else if (Vertices[J].Position == PreviousVertex.Position)
											{
												Indices.Add(J);
											}
											else if (Vertices[J].Position == NextVertex.Position)
											{
												Indices.Add(J);
											}
										}

										Math::Vec3D TempPosition{};
										for (uint32 J{}; J < static_cast<uint32>(TempVertices.Size()); ++J)
										{
											if (TempVertices[J].Position != CurrentVertex.Position &&
												TempVertices[J].Position != PreviousVertex.Position &&
												TempVertices[J].Position != NextVertex.Position)
											{
												TempPosition = TempVertices[J].Position;
												break;
											}
										}

										// create a triangle from Previous, Current and Next
										for (uint32 J{}; J < static_cast<uint32>(TempVertices.Size()); ++J)
										{
											if (Vertices[J].Position == TempPosition)
											{
												Indices.Add(J);
											}
											else if (Vertices[J].Position == PreviousVertex.Position)
											{
												Indices.Add(J);
											}
											else if (Vertices[J].Position == NextVertex.Position)
											{
												Indices.Add(J);
											}
										}

										TempVertices.Clear();
										break;
									}
								}
							}
						}

						OutVertices.AddRange(Vertices);

						for (const uint32 Index : Indices)
						{
							OutIndices.Add(static_cast<uint32>(OutVertices.Size() - Vertices.Size()) + Index);
						}
					}
					break;
					default:
						break;
				}
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