#pragma once

#include "EngineConstants.h"

#include "Array/Array.h"

#include "Component/Component.h"
#include "Components/TransformComponent/TransformComponent.h"
#include "TPair/TPair.h"

#include "Memory/UniquePtr.h"

#include "Vertex/Vertex.h"

#include <string>

DEFINE_LOG_CATEGORY(LogMeshComponent, Integrian3D::LogVerbosity::Verbose);

namespace Integrian3D
{
	class Light;
	class Texture;
	class Material;

	class MeshComponent final : public Component
	{
		// a submesh is part of a model, but we're naming the outer component a "mesh"
		struct SubMesh
		{
			SubMesh(const TArray<Vertex>& InVertices, const TArray<uint32>& InIndices, UniquePtr<Material>&& InMaterial);
			~SubMesh();

			SubMesh(const SubMesh&) noexcept = delete;
			SubMesh(SubMesh&& other) noexcept;
			SubMesh& operator=(const SubMesh&) noexcept = delete;
			SubMesh& operator=(SubMesh&& other) noexcept;

			void Render(const Math::Mat4D& Transform, const Math::Mat4D& View, const Math::Mat4D& Projection,
				const Math::Vec3D& CameraPosition, const TArray<TPair<TransformComponent, Light*>>& Lights);
			void SetupSubMesh();

			TArray<Vertex> Vertices;
			TArray<uint32> Indices;
			UniquePtr<Material> MeshMaterial;

			uint32 VertexArrayID;
			uint32 VertexBufferID;
			uint32 IndexBufferID;
		};

	public:
		MeshComponent(const std::string_view Filepath);
		~MeshComponent();

		MeshComponent(const MeshComponent&) noexcept = delete;
		MeshComponent(MeshComponent&& other) noexcept;
		MeshComponent& operator=(const MeshComponent&) noexcept = delete;
		MeshComponent& operator=(MeshComponent&& other) noexcept;

		void AddSubMesh(const TArray<Vertex>& InVertices, const TArray<uint32>& InIndices, UniquePtr<Material>&& InMaterial);
		void AddMaterial(const int32 Index, UniquePtr<Material>&& InMaterial);

		void Render(const Math::Mat4D& Transform, const Math::Mat4D& View, const Math::Mat4D& Projection,
			const Math::Vec3D& CameraPosition, const TArray<TPair<TransformComponent, Light*>>& Lights);

	private:
		TArray<UniquePtr<SubMesh>> SubMeshes;
	};
}