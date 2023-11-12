#include "MeshComponent.h"

#include "Material/Material.h"
#include "Texture/Texture.h"
#include "IO/PathUtils.h"

#include <glad/glad.h>

#include <gtx/vector_angle.hpp>

// Assimp stuff
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

namespace Integrian3D
{
	namespace
	{
		static void ProcessAssimpMesh(MeshComponent* const StaticMesh, aiMesh* const AssimpMesh, const aiScene* const AssimpScene)
		{
			TArray<Vertex> Vertices{};
			TArray<uint32> Indices{};

			for (uint32 i{}; i < AssimpMesh->mNumVertices; ++i)
			{
				Vertex Vertex{};

				Vertex.Position.x = AssimpMesh->mVertices[i].x;
				Vertex.Position.y = AssimpMesh->mVertices[i].y;
				Vertex.Position.z = AssimpMesh->mVertices[i].z;

				Vertex.Normal.x = AssimpMesh->mNormals[i].x;
				Vertex.Normal.y = AssimpMesh->mNormals[i].y;
				Vertex.Normal.z = AssimpMesh->mNormals[i].z;

				if (AssimpMesh->mTextureCoords[0])
				{
					Vertex.UV.x = AssimpMesh->mTextureCoords[0][i].x;
					Vertex.UV.y = AssimpMesh->mTextureCoords[0][i].y;
				}

				Vertices.Add(Vertex);
			}

			for (uint32 i{}; i < AssimpMesh->mNumFaces; ++i)
			{
				aiFace Face{ AssimpMesh->mFaces[i] };
				for (uint32 j{}; j < Face.mNumIndices; ++j)
				{
					Indices.Add(Face.mIndices[j]);
				}
			}

			// if (AssimpMesh->mMaterialIndex >= 0)
			// {
			// 	aiMaterial* const AssimpMaterial{ AssimpScene->mMaterials[AssimpMesh->mMaterialIndex] };
			// }

			StaticMesh->AddSubmesh(Vertices, Indices);
		}

		static void ProcessAssimpNode(MeshComponent* const StaticMesh, aiNode* const AssimpNode, const aiScene* const AssimpScene)
		{
			// process all the node's meshes, if any
			for (uint32 i{}; i < AssimpNode->mNumMeshes; ++i)
			{
				aiMesh* const AssimpMesh = AssimpScene->mMeshes[AssimpNode->mMeshes[i]];
			}

			// then do the same for each of its children
			for (uint32 i{}; i < AssimpNode->mNumChildren; ++i)
			{
				ProcessAssimpNode(StaticMesh, AssimpNode->mChildren[i], AssimpScene);
			}
		}
	}

	static constexpr uint32 MeshErrorValue = std::numeric_limits<uint32>::max();

	MeshComponent::MeshComponent(const std::string_view Filepath, UniquePtr<Material>&& Material)
	{
		if (!Filepath.empty() && PathUtils::DoesFileExist(Filepath))
		{
			Assimp::Importer Importer;
			const aiScene* const AssimpScene = Importer.ReadFile(Filepath.data(), aiProcess_Triangulate | aiProcess_FlipUVs);

			if (!AssimpScene || (AssimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !AssimpScene->mRootNode)
			{
				LOG(LogMeshComponent, Error, "Error loading 3D object: {}", Importer.GetErrorString());
				return;
			}

			ProcessAssimpNode(this, AssimpScene->mRootNode, AssimpScene);
		}
	}

	MeshComponent::~MeshComponent() {}

	MeshComponent::MeshComponent(MeshComponent&& other) noexcept
		: SubMeshes{ std::move(other.SubMeshes) }
	{}

	MeshComponent& MeshComponent::operator=(MeshComponent&& other) noexcept
	{
		SubMeshes = std::move(other.SubMeshes);

		return *this;
	}

	void MeshComponent::AddSubMesh(const TArray<Vertex>& InVertices, const TArray<uint32>& InIndices)
	{
		SubMeshes.EmplaceBack(InVertices, InIndices);
	}

	void MeshComponent::Render(const Math::Mat4D& Transform, const Math::Mat4D& View, const Math::Mat4D& Projection, const Math::Vec3D& CameraPosition, const TArray<TPair<TransformComponent, Light*>>& Lights)
	{
		for (UniquePtr<SubMesh>& SubMesh : SubMeshes)
		{
			SubMesh->Render(Transform, View, Projection, CameraPosition, Lights);
		}
	}

	MeshComponent::SubMesh::SubMesh(const TArray<Vertex>& InVertices, const TArray<uint32>& InIndices)
		: Vertices{ InVertices }
		, Indices{ InIndices }
		, Materials{}
		, VertexArrayID{ MeshErrorValue }
		, VertexBufferID{ MeshErrorValue }
		, IndexBufferID{ MeshErrorValue }
	{
		SetupSubMesh();
	}

	MeshComponent::SubMesh::~SubMesh()
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

	MeshComponent::SubMesh::SubMesh(SubMesh&& other) noexcept
		: VertexArrayID{ I_MOVE(other.VertexArrayID) }
		, VertexBufferID{ I_MOVE(other.VertexBufferID) }
		, IndexBufferID{ I_MOVE(other.IndexBufferID) }
		, Vertices{ I_MOVE(other.Vertices) }
		, Indices{ I_MOVE(other.Indices) }
		, Materials{ I_MOVE(other.Materials) }
	{
		other.Vertices.Clear();
		other.Indices.Clear();
		other.Materials.Clear();

		other.VertexArrayID = MeshErrorValue;
		other.VertexBufferID = MeshErrorValue;
		other.IndexBufferID = MeshErrorValue;
	}

	MeshComponent::SubMesh& MeshComponent::SubMesh::operator=(SubMesh&& other) noexcept
	{
		VertexArrayID = I_MOVE(other.VertexArrayID);
		VertexBufferID = I_MOVE(other.VertexBufferID);
		IndexBufferID = I_MOVE(other.IndexBufferID);
		Vertices = I_MOVE(other.Vertices);
		Indices = I_MOVE(other.Indices);
		Materials = I_MOVE(other.Materials);

		other.Vertices.Clear();
		other.Indices.Clear();
		other.Materials.Clear();

		other.VertexArrayID = MeshErrorValue;
		other.VertexBufferID = MeshErrorValue;
		other.IndexBufferID = MeshErrorValue;

		return *this;
	}

	void MeshComponent::SubMesh::Render(const Math::Mat4D& Transform, const Math::Mat4D& View, const Math::Mat4D& Projection, const Math::Vec3D& CameraPosition, const TArray<TPair<TransformComponent, Light*>>& Lights)
	{
		for (UniquePtr<Material>& Material : Materials)
		{
			Material->StartShader(Transform, View, Projection, CameraPosition, Lights);
		}

		/* Bind the Vertex Array ID */
		glBindVertexArray(VertexArrayID);

		/* Bind the ID to a vertex buffer */
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);

		/* Bind the ID to an index buffer */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);

		/* Render our rectangle */
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(Indices.Size()), GL_UNSIGNED_INT, 0);
	}

	void MeshComponent::SubMesh::SetupSubMesh()
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
		Normal Attribute:
		Stride = ???
		Offset = 12
		UV Attribute:
		Stride = 20
		Offset = 24
		*/

		/* Enable the Position Attribute */
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), /* offset = */ reinterpret_cast<void*>(0));

		/* Enable the Normal attribute */
		glEnableVertexAttribArray(1);

		/* Set Vertex Buffer Normal Attribute layout */
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), /* offset = */ reinterpret_cast<void*>(sizeof(Math::Vec3D)));

		/* Enable the UV Coord attribute */
		glEnableVertexAttribArray(2);

		/* Set Vertex Buffer UV Coord Attribute layout */
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), /* offset = */ reinterpret_cast<void*>(sizeof(Math::Vec3D) * 2));

		glBindVertexArray(0);
	}
}