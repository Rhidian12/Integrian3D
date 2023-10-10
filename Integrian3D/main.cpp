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
#include "Memory/UniquePtr.h"
#include "Components/TestRotateComponent/TestRotateComponent.h"
#include "Components/FreeCameraComponent/FreeCameraComponent.h"
#include "Material/Material.h"

int RunTestEngine(int, char* [])
{
	using namespace Integrian3D;

	Core& core{ Core::CreateCore(1080,720) };

	// TextureManager::GetInstance().AddTexture("__Wall", "Resources/wall.jpg");

	UniquePtr<Scene> TestScene{ new Scene{ "TestScene" } };
	SceneManager::GetInstance().AddScene(TestScene.Get());

	TArray<Vertex> vertices =
	{
		Vertex{ glm::vec3{ -0.5f, -0.5f, -0.5f }, Math::Vec3D{ 0.f, 0.f, -1.f }, glm::vec2{ 0.0f, 0.0f } },
		Vertex{ glm::vec3{  0.5f, -0.5f, -0.5f }, Math::Vec3D{ 0.f, 0.f, -1.f }, glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f, -0.5f }, Math::Vec3D{ 0.f, 0.f, -1.f }, glm::vec2{ 1.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f, -0.5f }, Math::Vec3D{ 0.f, 0.f, -1.f }, glm::vec2{ 1.0f, 1.0f } },
		Vertex{ glm::vec3{ -0.5f,  0.5f, -0.5f }, Math::Vec3D{ 0.f, 0.f, -1.f }, glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{ -0.5f, -0.5f, -0.5f }, Math::Vec3D{ 0.f, 0.f, -1.f }, glm::vec2{ 0.0f, 0.0f } },

		Vertex{ glm::vec3{ -0.5f, -0.5f,  0.5f }, Math::Vec3D{ 0.f, 0.f, 1.f }, glm::vec2{ 0.0f, 0.0f } },
		Vertex{ glm::vec3{  0.5f, -0.5f,  0.5f }, Math::Vec3D{ 0.f, 0.f, 1.f }, glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f,  0.5f }, Math::Vec3D{ 0.f, 0.f, 1.f }, glm::vec2{ 1.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f,  0.5f }, Math::Vec3D{ 0.f, 0.f, 1.f }, glm::vec2{ 1.0f, 1.0f } },
		Vertex{ glm::vec3{ -0.5f,  0.5f,  0.5f }, Math::Vec3D{ 0.f, 0.f, 1.f }, glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{ -0.5f, -0.5f,  0.5f }, Math::Vec3D{ 0.f, 0.f, 1.f }, glm::vec2{ 0.0f, 0.0f } },

		Vertex{ glm::vec3{ -0.5f,  0.5f,  0.5f }, Math::Vec3D{ -1.f, 0.f, 0.f }, glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{ -0.5f,  0.5f, -0.5f }, Math::Vec3D{ -1.f, 0.f, 0.f }, glm::vec2{ 1.0f, 1.0f } },
		Vertex{ glm::vec3{ -0.5f, -0.5f, -0.5f }, Math::Vec3D{ -1.f, 0.f, 0.f }, glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{ -0.5f, -0.5f, -0.5f }, Math::Vec3D{ -1.f, 0.f, 0.f }, glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{ -0.5f, -0.5f,  0.5f }, Math::Vec3D{ -1.f, 0.f, 0.f }, glm::vec2{ 0.0f, 0.0f } },
		Vertex{ glm::vec3{ -0.5f,  0.5f,  0.5f }, Math::Vec3D{ -1.f, 0.f, 0.f }, glm::vec2{ 1.0f, 0.0f } },

		Vertex{ glm::vec3{  0.5f,  0.5f,  0.5f }, Math::Vec3D{ 1.f, 0.f, 0.f }, glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f, -0.5f }, Math::Vec3D{ 1.f, 0.f, 0.f }, glm::vec2{ 1.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f, -0.5f, -0.5f }, Math::Vec3D{ 1.f, 0.f, 0.f }, glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f, -0.5f, -0.5f }, Math::Vec3D{ 1.f, 0.f, 0.f }, glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f, -0.5f,  0.5f }, Math::Vec3D{ 1.f, 0.f, 0.f }, glm::vec2{ 0.0f, 0.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f,  0.5f }, Math::Vec3D{ 1.f, 0.f, 0.f }, glm::vec2{ 1.0f, 0.0f } },

		Vertex{ glm::vec3{ -0.5f, -0.5f, -0.5f }, Math::Vec3D{ 0.f, -1.f, 0.f }, glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f, -0.5f, -0.5f }, Math::Vec3D{ 0.f, -1.f, 0.f }, glm::vec2{ 1.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f, -0.5f,  0.5f }, Math::Vec3D{ 0.f, -1.f, 0.f }, glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{  0.5f, -0.5f,  0.5f }, Math::Vec3D{ 0.f, -1.f, 0.f }, glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{ -0.5f, -0.5f,  0.5f }, Math::Vec3D{ 0.f, -1.f, 0.f }, glm::vec2{ 0.0f, 0.0f } },
		Vertex{ glm::vec3{ -0.5f, -0.5f, -0.5f }, Math::Vec3D{ 0.f, -1.f, 0.f }, glm::vec2{ 0.0f, 1.0f } },

		Vertex{ glm::vec3{ -0.5f,  0.5f, -0.5f }, Math::Vec3D{ 0.f, 1.f, 0.f }, glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f, -0.5f }, Math::Vec3D{ 0.f, 1.f, 0.f }, glm::vec2{ 1.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f,  0.5f }, Math::Vec3D{ 0.f, 1.f, 0.f }, glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f,  0.5f }, Math::Vec3D{ 0.f, 1.f, 0.f }, glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{ -0.5f,  0.5f,  0.5f }, Math::Vec3D{ 0.f, 1.f, 0.f }, glm::vec2{ 0.0f, 0.0f } },
		Vertex{ glm::vec3{ -0.5f,  0.5f, -0.5f }, Math::Vec3D{ 0.f, 1.f, 0.f }, glm::vec2{ 0.0f, 1.0f } }
	};

	TArray<uint32_t> indices{};

	for (uint32_t i{}; i < vertices.Size(); ++i)
	{
		indices.Add(i);
	}

	{
		Entity entity{ TestScene->CreateEntity() };
		// TestScene->AddComponent<TestRotateComponent>(entity);

		UniquePtr<Material> MeshMaterial = MakeUnique<Material>("Resources/LightVertexShader.txt", "Resources/LightFragmentShader.txt");
		MeshMaterial->SetVec3("_Material.Ambient", Math::Vec3D{ 1.0f, 0.5f, 0.3f });
		MeshMaterial->SetVec3("_Material.Diffuse", Math::Vec3D{ 1.0f, 0.5f, 0.3f });
		MeshMaterial->SetVec3("_Material.Specular", Math::Vec3D{ 0.5f, 0.5f, 0.5f });
		MeshMaterial->SetFloat("_Material.Shininess", 32.f);
		MeshMaterial->SetVec3("_LightColor", Math::Vec3D{ 1.0f, 1.0f, 1.0f });
		MeshMaterial->SetVec3("_LightPos", Math::Vec3D{ 1.2f,1.0f,-2.0f });
		TestScene->AddComponent<MeshComponent>(entity, vertices, indices, __MOVE(MeshMaterial));
	}

	{
		Entity Entity{ TestScene->CreateEntity() };

		UniquePtr<Material> MeshMaterial = MakeUnique<Material>("Resources/LightVertexShader2.txt", "Resources/LightFragmentShader2.txt");
		TestScene->AddComponent<MeshComponent>(Entity, vertices, indices, __MOVE(MeshMaterial));

		TestScene->GetComponent<TransformComponent>(Entity).Translate(Math::Vec3D{ 1.2f,1.0f,-2.0f });
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