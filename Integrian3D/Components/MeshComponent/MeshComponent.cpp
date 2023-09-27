#include "MeshComponent.h"

#include "IO/File/File.h"
#include "Renderer/Renderer.h"
#include "Texture/Texture.h"

I_DISABLE_WARNING(4201)
#include <gtx/vector_angle.hpp>
I_ENABLE_WARNING(4201)
#include <raylib.h>

namespace Integrian3D
{
	static constexpr uint32 MeshErrorValue = std::numeric_limits<uint32>::max();

	MeshComponent::MeshComponent(const std::string_view Filepath, Texture* const pTex)
		: ModelImplementation{}
		, pTexture{ pTex }
	{
		if (!Filepath.empty())
		{
			ModelImplementation = MakeUnique<Model>(LoadModel(Filepath.data()));
		}
	}

	MeshComponent::MeshComponent(MeshComponent&& other) noexcept
		: ModelImplementation{ __MOVE(other.ModelImplementation) }
		, pTexture{ __MOVE(other.pTexture) }
	{
		other.pTexture = nullptr;
	}

	MeshComponent& MeshComponent::operator=(MeshComponent&& other) noexcept
	{
		ModelImplementation = __MOVE(other.ModelImplementation);
		pTexture = __MOVE(other.pTexture);

		other.pTexture = nullptr;

		return *this;
	}

	const Model* const MeshComponent::GetRayLibModel() const
	{
		return ModelImplementation.Get();
	}
}