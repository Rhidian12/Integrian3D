#include "MeshComponent.h"

#include "IO/File/File.h"
#include "Material/Material.h"
#include "Renderer/Renderer.h"
#include "Texture/Texture.h"

#include <glad/glad.h>
#include <gtx/vector_angle.hpp>
#include <sstream>

namespace Integrian3D
{
	static constexpr uint32 MeshErrorValue = std::numeric_limits<uint32>::max();

	MeshComponent::MeshComponent(const std::string_view Filepath, UniquePtr<Material>&& Material)
		: VertexArrayID{ MeshErrorValue }
		, VertexBufferID{ MeshErrorValue }
		, IndexBufferID{ MeshErrorValue }
		, Vertices{}
		, Indices{}
		, MeshMaterial{ __MOVE(Material) }
	{
		if (!Filepath.empty())
		{
			__ASSERT(Filepath.substr(Filepath.find_last_of('.')) == ".obj", "MeshComponent can only deserialize .obj files currently");

			// ParseObjFile(Filepath, Vertices, Indices);

			SetupMesh();
		}
	}

	MeshComponent::MeshComponent(const TArray<Vertex>& vertices, const TArray<uint32>& indices, UniquePtr<Material>&& Material)
		: VertexArrayID{ MeshErrorValue }
		, VertexBufferID{ MeshErrorValue }
		, IndexBufferID{ MeshErrorValue }
		, Vertices{ vertices }
		, Indices{ indices }
		, MeshMaterial{ __MOVE(Material) }
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
		, MeshMaterial{ __MOVE(other.MeshMaterial) }
	{
		other.Vertices.Clear();
		other.Indices.Clear();

		other.VertexArrayID = MeshErrorValue;
		other.VertexBufferID = MeshErrorValue;
		other.IndexBufferID = MeshErrorValue;
	}

	MeshComponent& MeshComponent::operator=(MeshComponent&& other) noexcept
	{
		VertexArrayID = __MOVE(other.VertexArrayID);
		VertexBufferID = __MOVE(other.VertexBufferID);
		IndexBufferID = __MOVE(other.IndexBufferID);
		Vertices = __MOVE(other.Vertices);
		Indices = __MOVE(other.Indices);
		MeshMaterial = __MOVE(other.MeshMaterial);

		other.Vertices.Clear();
		other.Indices.Clear();

		other.VertexArrayID = MeshErrorValue;
		other.VertexBufferID = MeshErrorValue;
		other.IndexBufferID = MeshErrorValue;

		return *this;
	}

	void MeshComponent::StartShader(const Math::Mat4D& Transform, const Math::Mat4D& View, const Math::Mat4D& Projection, const Math::Vec3D& CameraPosition) const
	{
		MeshMaterial->StartShader(Transform, View, Projection, CameraPosition);
	}

	Material* const MeshComponent::GetMaterial()
	{
		return MeshMaterial.Get();
	}

	const Material* const MeshComponent::GetMaterial() const
	{
		return MeshMaterial.Get();
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

		/* Set Vertex Buffer UV Coord Attribute layout */
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), /* offset = */ reinterpret_cast<void*>(sizeof(Math::Vec3D)));

		/* Enable the UV Coord attribute */
		glEnableVertexAttribArray(2);

		/* Set Vertex Buffer UV Coord Attribute layout */
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), /* offset = */ reinterpret_cast<void*>(sizeof(Math::Vec3D) * 2));

		glBindVertexArray(0);
	}
}