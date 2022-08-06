#pragma once

#include <functional> /* std::function */

namespace Integrian3D
{
	class Scene final
	{
	public:
		explicit Scene();

	private:
		std::function<void()> InitializeCallback;
	};
}