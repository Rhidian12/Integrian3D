#if __has_include(<vld.h>)
#	include <vld.h>
#endif

// #define ENGINE
// #define UNIT_TESTS
#define TEST_ENGINE

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
#include "Components/TestRotateComponent/TestRotateComponent.h"
#include "Components/FreeCameraComponent/FreeCameraComponent.h"

int RunTestEngine(int, char* [])
{
	using namespace Integrian3D;
	using namespace Integrian3D::Memory;

	Core& core{ Core::CreateCore(1080,720) };

	TArray<Vertex> vertices =
	{
		Vertex{ glm::vec3{ -0.5f, -0.5f, -0.5f },  glm::vec2{ 0.0f, 0.0f } },
		Vertex{ glm::vec3{  0.5f, -0.5f, -0.5f },  glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f, -0.5f },  glm::vec2{ 1.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f, -0.5f },  glm::vec2{ 1.0f, 1.0f } },
		Vertex{ glm::vec3{ -0.5f,  0.5f, -0.5f },  glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{ -0.5f, -0.5f, -0.5f },  glm::vec2{ 0.0f, 0.0f } },

		Vertex{ glm::vec3{ -0.5f, -0.5f,  0.5f },  glm::vec2{ 0.0f, 0.0f } },
		Vertex{ glm::vec3{  0.5f, -0.5f,  0.5f },  glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f,  0.5f },  glm::vec2{ 1.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f,  0.5f },  glm::vec2{ 1.0f, 1.0f } },
		Vertex{ glm::vec3{ -0.5f,  0.5f,  0.5f },  glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{ -0.5f, -0.5f,  0.5f },  glm::vec2{ 0.0f, 0.0f } },

		Vertex{ glm::vec3{ -0.5f,  0.5f,  0.5f },  glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{ -0.5f,  0.5f, -0.5f },  glm::vec2{ 1.0f, 1.0f } },
		Vertex{ glm::vec3{ -0.5f, -0.5f, -0.5f },  glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{ -0.5f, -0.5f, -0.5f },  glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{ -0.5f, -0.5f,  0.5f },  glm::vec2{ 0.0f, 0.0f } },
		Vertex{ glm::vec3{ -0.5f,  0.5f,  0.5f },  glm::vec2{ 1.0f, 0.0f } },

		Vertex{ glm::vec3{  0.5f,  0.5f,  0.5f },  glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f, -0.5f },  glm::vec2{ 1.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f, -0.5f, -0.5f },  glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f, -0.5f, -0.5f },  glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f, -0.5f,  0.5f },  glm::vec2{ 0.0f, 0.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f,  0.5f },  glm::vec2{ 1.0f, 0.0f } },

		Vertex{ glm::vec3{ -0.5f, -0.5f, -0.5f },  glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f, -0.5f, -0.5f },  glm::vec2{ 1.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f, -0.5f,  0.5f },  glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{  0.5f, -0.5f,  0.5f },  glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{ -0.5f, -0.5f,  0.5f },  glm::vec2{ 0.0f, 0.0f } },
		Vertex{ glm::vec3{ -0.5f, -0.5f, -0.5f },  glm::vec2{ 0.0f, 1.0f } },

		Vertex{ glm::vec3{ -0.5f,  0.5f, -0.5f },  glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f, -0.5f },  glm::vec2{ 1.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f,  0.5f },  glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f,  0.5f },  glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{ -0.5f,  0.5f,  0.5f },  glm::vec2{ 0.0f, 0.0f } },
		Vertex{ glm::vec3{ -0.5f,  0.5f, -0.5f },  glm::vec2{ 0.0f, 1.0f } }
	};

	TArray<uint32_t> indices{};

	for (uint32_t i{}; i < vertices.Size(); ++i)
	{
		indices.Add(i);
	}

	TextureManager::GetInstance().AddTexture("__Wall", "Resources/wall.jpg");

	Scene* pTestScene{ new Scene{ "TestScene" } };
	SceneManager::GetInstance().AddScene(pTestScene);

	{
		Entity entity{ pTestScene->CreateEntity() };
		pTestScene->AddComponent<TestRotateComponent>(entity);
		pTestScene->AddComponent<MeshComponent>(entity, vertices, indices, TextureManager::GetInstance().GetTexture("__Wall"));
		// pTestScene->GetComponent<TransformComponent>(entity).Rotate(Math::Vec3D{ Math::ToRadians(-55.f), 0.f, 0.f });
	}

	for (size_t i{}; i < 9; ++i)
	{
		Entity entity{ pTestScene->CreateEntity() };
		pTestScene->AddComponent<TestRotateComponent>(entity);
		pTestScene->AddComponent<MeshComponent>(entity, vertices, indices, TextureManager::GetInstance().GetTexture("__Wall"));
		// pTestScene->GetComponent<TransformComponent>(entity).Rotate(Math::Vec3D{ Math::ToRadians(-55.f), 0.f, 0.f });
		pTestScene->GetComponent<TransformComponent>(entity).Translate(Math::RandomVec3D(-5.f, 5.f));
	}

	pTestScene->AddUpdateCallback(0, [](Scene& scene)->void
		{
			scene.CreateView<TestRotateComponent, TransformComponent>().ForEach([](TestRotateComponent& rotate, TransformComponent& transform)->void
				{
					rotate.Rotate(transform);
				});
		});

	core.Run();

	delete pTestScene;

	return 0;
}
#endif