#include "Renderer.h"

namespace Integrian3D
{
	Renderer::Renderer()
	{}

	Renderer& Renderer::GetInstance()
	{
		if (!Instance)
		{
			Instance = std::make_unique<Renderer>();
		}

		return *Instance.get();
	}
}