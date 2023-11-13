#pragma once

#include "EngineConstants.h"

namespace Integrian3D
{
	class MeshComponent;
}

struct aiNode;
struct aiScene;
struct aiMesh;

namespace Integrian3D::AssimpWrapper
{
	bool LoadModel(MeshComponent* const StaticMesh, const char* Filepath, std::string& OutError);
	void ProcessAssimpNode(MeshComponent* const StaticMesh, aiNode* const AssimpNode, const aiScene* const AssimpScene);
	void ProcessAssimpMesh(MeshComponent* const StaticMesh, aiMesh* const AssimpMesh, const aiScene* const AssimpScene);
}