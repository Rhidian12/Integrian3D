#pragma once

#include "../ECS/Registry/Registry.h"

#include <functional> /* std::function */

namespace Integrian3D
{
	class Scene final
	{
	public:
		explicit Scene();

		void Start();

		void AddSceneInitialisation(const std::function<void(const Scene&)>& fn) { InitializeCallback = fn; }

	private:
		std::function<void(const Scene&)> InitializeCallback;
		Registry Registry;
	};
}