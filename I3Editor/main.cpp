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
	const float Width = static_cast<float>(CALL_WIN32_RV(GetSystemMetrics(SM_CXFULLSCREEN)));
	const float Height = static_cast<float>(CALL_WIN32_RV(GetSystemMetrics(SM_CYFULLSCREEN)));

	Core& EditorCore{ Core::CreateCore(static_cast<int32>(Width), static_cast<int32>(Height)) };

	Scene* const EditorScene{ SceneManager::GetInstance().AddScene<Scene>("Editor") };

	constexpr ImGuiWindowFlags ImGuiNoResizeAndNoMove = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

	#pragma region Top Bar
	const float TopBarWidth = Width;
	constexpr float TopBarHeight = 75.f;

	EditorScene->AddRenderCallback(UpdateOrder::CoreUpdate, [TopBarWidth, TopBarHeight](const Scene& Scene)->void
		{
			ImGui::SetNextWindowPos(ImVec2{});
			ImGui::SetNextWindowSize(ImVec2{ TopBarWidth, TopBarHeight });
			if (ImGui::Begin("I3Editor", nullptr, ImGuiNoResizeAndNoMove))
			{
				if (ImGui::Button("Add GameObject"))
				{
					LOG(Log, LogErrorLevel::Log, "ADDED GAMEOBJECT");
				}
			}

			ImGui::End();
		});
	#pragma endregion

	#pragma region World Outliner
	constexpr float OutlinerWidth = 300.f;
	constexpr float OutlinerHeight = 300.f;

	EditorScene->AddRenderCallback(UpdateOrder::CoreUpdate, [OutlinerWidth, OutlinerHeight, Width, TopBarHeight](const Scene& Scene)->void
		{
			ImGui::SetNextWindowPos(ImVec2{ Width - OutlinerWidth, TopBarHeight });
			ImGui::SetNextWindowSize(ImVec2{ OutlinerWidth, OutlinerHeight });
			if (ImGui::Begin("World Outliner", nullptr, ImGuiNoResizeAndNoMove))
			{
				if (ImGui::BeginListBox("", ImVec2{ 300.f, 250.f }))
				{

				}

				ImGui::EndListBox();
			}

			ImGui::End();
		});
	#pragma endregion

	#pragma region Component Information
	constexpr float ComponentInfoWidth = 300.f;
	const float ComponentInfoHeight = Height - OutlinerHeight - TopBarHeight;

	EditorScene->AddRenderCallback(UpdateOrder::CoreUpdate, [ComponentInfoWidth, ComponentInfoHeight, Width, TopBarHeight, OutlinerHeight](const Scene& Scene)->void
		{
			ImGui::SetNextWindowPos(ImVec2{ Width - ComponentInfoWidth, TopBarHeight + OutlinerHeight });
			ImGui::SetNextWindowSize(ImVec2{ ComponentInfoWidth, ComponentInfoHeight });
			if (ImGui::Begin("Component Info", nullptr, ImGuiNoResizeAndNoMove))
			{

			}

			ImGui::End();
		});
	#pragma endregion

	EditorCore.Run();
}