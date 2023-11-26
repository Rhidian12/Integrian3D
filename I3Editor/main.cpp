#include "Core/Core.h"
#include "Scene/Scene.h"
#include "SceneManager/SceneManager.h"
#include "Win32Utils/Win32APICall.h"

#include <imgui.h>

#if _WIN32
#	include <Windows.h>
#endif

int main(int argc, char* argv[])
{
	using namespace Integrian3D;

	// size of screen in pixels excluding size of taskbar
	const int32 Width = CALL_WIN32_RV(GetSystemMetrics(SM_CXFULLSCREEN));
	const int32 Height = CALL_WIN32_RV(GetSystemMetrics(SM_CYFULLSCREEN));

	Core& EditorCore{ Core::CreateCore(Width, Height) };

	Scene* const EditorScene{ SceneManager::GetInstance().AddScene<Scene>("Editor") };

	EditorScene->AddRenderCallback(UpdateOrder::CoreUpdate, [](const Scene& Scene)->void
		{
			{
				ImGui::SetNextWindowSize(ImVec2{ 100,100 });
				if (ImGui::Begin("I3Editor"))
				{
					if (ImGui::Button("Add GameObject"))
					{
						LOG(Log, LogErrorLevel::Log, "ADDED GAMEOBJECT");
					}
				}

				ImGui::End();
			}
		});

	EditorCore.Run();
}