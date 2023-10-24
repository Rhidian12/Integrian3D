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
#include "Light/DirectionalLight.h"
#include "Light/PointLight.h"
#include "Renderer/Renderer.h"

int RunTestEngine(int, char* [])
{
	using namespace Integrian3D;

	Core& core{ Core::CreateCore(1080,720) };

	// IO::File File{ "Resources/LightFragmentShader.frag", IO::OpenMode::OpenExisting, IO::FileMode::ASCII, true };

	TextureManager::GetInstance().AddTexture("Box_Diffuse", "Resources/box.png");
	TextureManager::GetInstance().AddTexture("Box_Specular", "Resources/box_specular.png");

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
		Entity Entity{ TestScene->CreateEntity() };
		// TestScene->AddComponent<TestRotateComponent>(Entity);

		UniquePtr<Material> MeshMaterial = MakeUnique<Material>("Resources/LightVertexShader.vert", "Resources/LightFragmentShader.frag");
		MeshMaterial->SetFloat("_Material.Shininess", 32.f);

		MeshMaterial->AddTexture(TextureSlots::Diffuse, TextureManager::GetInstance().GetTexture("Box_Diffuse"));
		MeshMaterial->AddTexture(TextureSlots::Specular, TextureManager::GetInstance().GetTexture("Box_Specular"));

		TestScene->AddComponent<MeshComponent>(Entity, vertices, indices, __MOVE(MeshMaterial));
	}

	{
		const Entity PointLightEntity{ TestScene->CreateEntity() };

		const Math::Vec3D Ambient{ 0.2f, 0.2f, 0.2f };
		const Math::Vec3D Diffuse{ 0.5f, 0.5f, 0.5f };
		const Math::Vec3D Specular { 1.f, 1.f, 1.f };

		UniquePtr<Material> MeshMaterial = MakeUnique<Material>("Resources/LightVertexShader2.vert", "Resources/LightFragmentShader2.frag");
		TestScene->AddComponent<MeshComponent>(PointLightEntity, vertices, indices, __MOVE(MeshMaterial));
		TestScene->AddComponent<PointLight>(PointLightEntity, Ambient, Diffuse, Specular, 1000.f);
		TestScene->GetComponent<TransformComponent>(PointLightEntity).Translate(Math::Vec3D{ 1.2f, 1.0f, -2.0f });

		// TestScene->AddComponent<DirectionalLight>(PointLightEntity, Ambient, Diffuse, Specular, Math::Vec3D{ -0.2f, -1.0f, -0.3f });
	}

	{
		//for (int32 i{}; i < 10; ++i)
		//{
		//	const Entity Entity{ TestScene->CreateEntity() };

		//	TestScene->AddComponent<TestRotateComponent>(Entity);

		//	UniquePtr<Material> MeshMaterial = MakeUnique<Material>("Resources/LightVertexShader.vert", "Resources/LightFragmentShader.frag");
		//	MeshMaterial->SetFloat("_Material.Shininess", 32.f);

		//	MeshMaterial->AddTexture(TextureSlots::Diffuse, TextureManager::GetInstance().GetTexture("Box_Diffuse"));
		//	MeshMaterial->AddTexture(TextureSlots::Specular, TextureManager::GetInstance().GetTexture("Box_Specular"));

		//	TestScene->AddComponent<MeshComponent>(Entity, vertices, indices, __MOVE(MeshMaterial));

		//	TestScene->GetComponent<TransformComponent>(Entity).Translate(Math::RandomVec3D(-10.f, 10.f));
		//}
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