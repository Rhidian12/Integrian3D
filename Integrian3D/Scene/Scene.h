#pragma once

#include <functional> /* std::function */

namespace Integrian3D
{
	class Scene final
	{
	public:
		explicit Scene();

		void Start();

		void AddSceneInitialisation(const std::function<void()>& fn) { InitializeCallback = fn; }

	private:
		std::function<void()> InitializeCallback;
	};
}