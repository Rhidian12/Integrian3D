#include "Core/Core.h"
#include "Scene/Scene.h"
#include "SceneManager/SceneManager.h"
#include "Win32Utils/Win32APICall.h"

#include <imgui.h>

#if _WIN32
#	include <Windows.h>
#endif

#define CHECK_NOT_ZERO(x) if (x != 0) { __debugbreak(); }

int main(int argc, char* argv[])
{
	using namespace Integrian3D;

	// size of screen in pixels excluding size of taskbar
	int Width = CALL_WIN32_RV(GetSystemMetrics(SM_CXFULLSCREEN));
	int Height = CALL_WIN32_RV(GetSystemMetrics(SM_CYFULLSCREEN));

	Core& EditorCore{ Core::CreateCore(Width, Height) };

	Scene* TestScene{ SceneManager::GetInstance().AddScene<Scene>("TestScene") };

	TestScene->AddRenderCallback(UpdateOrder::CoreUpdate, [](const Scene& Scene)->void
		{
			auto error = GetLastError();
			CHECK_NOT_ZERO(error);
			{
				ImGui::SetNextWindowSize(ImVec2{ 100,100 });
			auto error2 = GetLastError();
			CHECK_NOT_ZERO(error2);
				if (ImGui::Begin("I3Editor"))
				{
			auto error3 = GetLastError();
			CHECK_NOT_ZERO(error3);
					if (ImGui::Button("Add GameObject"))
					{
						LOG(Log, LogErrorLevel::Log, "ADDED GAMEOBJECT");
					}
			auto error4 = GetLastError();
			CHECK_NOT_ZERO(error4);
				}

				ImGui::End();
			auto error5 = GetLastError();
			CHECK_NOT_ZERO(error5);
			}
		});

	EditorCore.Run();
}