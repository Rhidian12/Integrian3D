#include "Core/Core.h"

#include <imgui.h>

#if _WIN32
#	include <Windows.h>
#endif

int main(int argc, char* argv[])
{
	using namespace Integrian3D;

	// size of screen in pixels excluding size of taskbar
	int Width = GetSystemMetrics(SM_CXFULLSCREEN);
	int Height = GetSystemMetrics(SM_CYFULLSCREEN);

	Core& EditorCore{ Core::CreateCore(Width, Height) };

	EditorCore.Run();
}