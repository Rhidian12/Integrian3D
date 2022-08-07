#include "Scene.h"

namespace Integrian3D
{
	Scene::Scene()
		: InitializeCallback{}
	{
	}

	void Scene::Start()
	{
		InitializeCallback(*this);
	}
}