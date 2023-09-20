#if __has_include(<vld.h>)
#	include <vld.h>
#endif

// #define ENGINE
#define UNIT_TESTS
// #define TEST_ENGINE

#ifdef ENGINE

int RunEngine(int argc, char* argv[]);

#elif defined UNIT_TESTS

int RunUnitTests(int argc, char* argv[]);

#elif defined TEST_ENGINE

int RunTestEngine(int argc, char* argv[]);

#endif

int main(int argc, char* argv[])
{
	#ifdef ENGINE

	return RunEngine(argc, argv);

	#elif defined UNIT_TESTS

	return RunUnitTests(argc, argv);

	#elif defined TEST_ENGINE

	return RunTestEngine(argc, argv);

	#endif
}

#ifdef TEST_ENGINE
#include "EngineConstants.h"
#include "Core/Core.h"
#include "Scene/Scene.h"
#include "SceneManager/SceneManager.h"
#include "Components/MeshComponent/MeshComponent.h"
#include "TextureManager/TextureManager.h"
#include "Components/TransformComponent/TransformComponent.h"
#include "Timer/Timer.h"
#include "Memory/Allocator/Allocator.h"
#include "Memory/UniquePtr.h"
#include "Components/TestRotateComponent/TestRotateComponent.h"
#include "Components/FreeCameraComponent/FreeCameraComponent.h"

int RunTestEngine(int, char* [])
{
	using namespace Integrian3D;

	Core& core{ Core::CreateCore(1080,720) };

	TextureManager::GetInstance().AddTexture("__Wall", "Resources/wall.jpg");

	UniquePtr<Scene> TestScene{ new Scene{ "TestScene" } };
	SceneManager::GetInstance().AddScene(TestScene.Get());

	{
		Entity entity{ TestScene->CreateEntity() };
		TestScene->AddComponent<TestRotateComponent>(entity);
		TestScene->AddComponent<MeshComponent>(entity, "Resources/Spider.obj", TextureManager::GetInstance().GetTexture("__Wall"));
		TestScene->GetComponent<TransformComponent>(entity).Rotate(Math::Vec3D{ 0.f, Math::ToRadians(180.f), 0.f });
	}

	TestScene->AddUpdateCallback(0, [](Scene& scene)->void
		{
			if (scene.CanViewBeCreated<TestRotateComponent, TransformComponent>())
			{
				scene.CreateView<TestRotateComponent, TransformComponent>().ForEach([](TestRotateComponent& rotate, TransformComponent& transform)->void
					{
						rotate.Rotate(transform);
					});
			}
		});

	core.Run();

	return 0;
}
#endif