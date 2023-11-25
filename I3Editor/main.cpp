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

	char Buffer[MAX_PATH]{};
	GetCurrentDirectoryA(MAX_PATH, Buffer);

	LOG(Log, LogErrorLevel::Log, "Current Directory: {}", Buffer);

	Core& EditorCore{ Core::CreateCore(Width, Height) };

	EditorCore.Run();
}