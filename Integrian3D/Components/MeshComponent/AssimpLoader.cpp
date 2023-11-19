#include "Components/MeshComponent/AssimpLoader.h"
#include "Components/MeshComponent/MeshComponent.h"
#include "TextureManager/TextureManager.h"

#include "Memory/UniquePtr.h"
#include "Material/Material.h"

// Assimp stuff
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Integrian3D::AssimpWrapper
{
	void ProcessAssimpMesh(MeshComponent* const StaticMesh, aiMesh* const AssimpMesh, const aiScene* const AssimpScene)
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

		UniquePtr<Material> MeshMaterial{};
		// [CRINGE]: Only 1 material per mesh
		if (AssimpMesh->mMaterialIndex >= 0)
		{
			aiMaterial* const AssimpMaterial{ AssimpScene->mMaterials[AssimpMesh->mMaterialIndex] };

			aiString Str;
			auto test = AssimpMaterial->Get(AI_MATKEY_NAME, Str);

			// [CRINGE]: Only 1 map allowed per type
			if (AssimpMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				aiString str;
				AssimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &str);

				// TextureManager::GetInstance().AddTexture("")
			}

			if (AssimpMaterial->GetTextureCount(aiTextureType_BASE_COLOR) > 0)
			{
				aiString str;
				AssimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &str);

				// TextureManager::GetInstance().AddTexture("")
			}

			if (AssimpMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0)
			{
				aiString str;
				AssimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &str);

				// TextureManager::GetInstance().AddTexture("")
			}
		}

		if (!MeshMaterial)
		{
			MeshMaterial = MakeUnique<Material>("Resources/DefaultShader.vert", "Resources/DefaultShader.frag");
		}

		StaticMesh->AddSubMesh(Vertices, Indices, std::move(MeshMaterial));
	}

	bool LoadModel(MeshComponent* const StaticMesh, const char* Filepath, std::string& OutError)
	{
		Assimp::Importer Importer;
		const aiScene* const AssimpScene = Importer.ReadFile(Filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!AssimpScene || (AssimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !AssimpScene->mRootNode)
		{
			OutError = Importer.GetErrorString();
			return false;
		}

		ProcessAssimpNode(StaticMesh, AssimpScene->mRootNode, AssimpScene);

		for (uint32 i{}; i < AssimpScene->mRootNode->mNumChildren; ++i)
		{
			aiNode* AssimpNode = AssimpScene->mRootNode->mChildren[i];
			aiMatrix4x4 Transform = AssimpNode->mTransformation;
			aiNode* Node{ AssimpNode->mParent };

			while (Node != nullptr)
			{
				Transform *= Node->mTransformation;
				Node = Node->mParent;
			}

			Math::Mat4D TransformationGLM
			{
				Transform.a1, Transform.b1, Transform.c1, Transform.d1,
				Transform.a2, Transform.b2, Transform.c2, Transform.d2,
				Transform.a3, Transform.b3, Transform.c3, Transform.d3,
				Transform.a4, Transform.b4, Transform.c4, Transform.d4
			};

			StaticMesh->TransformSubMesh(i, TransformationGLM);

			LOG(Log, LogErrorLevel::Log, "Transform: {}, {}. {}", Transform.a4, Transform.b4, Transform.c4);
		}

		return true;
	}

	void ProcessAssimpNode(MeshComponent* const StaticMesh, aiNode* const AssimpNode, const aiScene* const AssimpScene)
	{
		// process all the node's meshes, if any
		for (uint32 i{}; i < AssimpNode->mNumMeshes; ++i)
		{
			aiMesh* const AssimpMesh = AssimpScene->mMeshes[AssimpNode->mMeshes[i]];
			ProcessAssimpMesh(StaticMesh, AssimpMesh, AssimpScene);
		}

		// then do the same for each of its children
		for (uint32 i{}; i < AssimpNode->mNumChildren; ++i)
		{
			ProcessAssimpNode(StaticMesh, AssimpNode->mChildren[i], AssimpScene);
		}
	}
}