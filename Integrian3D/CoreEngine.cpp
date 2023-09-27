#include "Core/Core.h"

int RunEngine(int, char*[])
{
	using namespace Integrian3D;

	Core core{ 1080,720 };

	core.Run();

	return 0;
}