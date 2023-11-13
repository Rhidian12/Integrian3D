#if __has_include(<vld.h>)
#	include <vld.h>
#	define USE_CRTDBG 0
#else
#	define USE_CRTDBG 1
#	define _CRTDBG_MAP_ALLOC
#	include <stdlib.h>
#	include <crtdbg.h>
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
#if USE_CRTDBG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

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
#include "InputManager/InputManager.h"

int RunTestEngine(int, char* [])
{
	using namespace Integrian3D;

	Core& core{ Core::CreateCore(1080,720) };

	TextureManager::GetInstance().AddTexture("Box_Diffuse", "Resources/box.png");
	TextureManager::GetInstance().AddTexture("Box_Specular", "Resources/box_specular.png");
	TextureManager::GetInstance().AddTexture("Cube_1_Diffuse", "Resources/T_Cube1.png");

	Scene* TestScene{ SceneManager::GetInstance().AddScene<Scene>("TestScene") };

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

	TArray<uint32> indices{};

	for (int32_t i{}; i < vertices.Size(); ++i)
	{
		indices.Add(static_cast<uint32>(i));
	}

	{
		for (int32 i{}; i < 3; ++i)
		{
			const Entity TestEntity{ TestScene->CreateEntity() };

			UniquePtr<Material> MeshMaterial = MakeUnique<Material>("Resources/LightVertexShader.vert", "Resources/LightFragmentShader.frag");
			MeshMaterial->SetFloat("_Material.Shininess", 32.f);

			MeshMaterial->AddTexture(TextureSlots::Diffuse, TextureManager::GetInstance().GetTexture("Cube_1_Diffuse"));
			// MeshMaterial->AddTexture(TextureSlots::Specular, TextureManager::GetInstance().GetTexture("Box_Specular"));

			MeshComponent& Cube_1 = TestScene->AddComponent<MeshComponent>(TestEntity, "Resources/Cube_1.obj");
			Cube_1.AddMaterial(0, std::move(MeshMaterial));

			TestScene->GetComponent<TransformComponent>(TestEntity).Translate(Math::Vec3D{ i * 101.f, 0.f, -100.f });
		}
	}

	const Entity PointLightEntity{ TestScene->CreateEntity() };
	{
		const Math::Vec3D Ambient{ 0.2f, 0.2f, 0.2f };
		const Math::Vec3D Diffuse{ 0.5f, 0.5f, 0.5f };
		const Math::Vec3D Specular{ 1.f, 1.f, 1.f };

		UniquePtr<Material> MeshMaterial = MakeUnique<Material>("Resources/LightVertexShader2.vert", "Resources/LightFragmentShader2.frag");
		// TestScene->AddComponent<MeshComponent>(PointLightEntity, vertices, indices, I_MOVE(MeshMaterial));
		// TestScene->AddComponent<PointLight>(PointLightEntity, Ambient, Diffuse, Specular, 1.f, 40.f);
		// TestScene->GetComponent<TransformComponent>(PointLightEntity).Translate(Math::Vec3D{ 1.2f, 1.0f, -2.0f });

		TestScene->AddComponent<DirectionalLight>(PointLightEntity, Ambient, Diffuse, Specular, Math::Vec3D{ -0.2f, -1.0f, -0.3f });
	}

	//float MaxRadius{ TestScene->GetComponent<PointLight>(PointLightEntity).GetMaxRadius() };
	//TestScene->AddUpdateCallback(0, [&MaxRadius, PointLightEntity](Scene& Scene)->void
	//	{
	//		const float step{ 0.025f };
	//		if (InputManager::GetInstance().GetIsKeyPressed(KeyboardInput::Up))
	//		{
	//			MaxRadius += step;

	//			Scene.GetComponent<PointLight>(PointLightEntity).SetMaxRadius(MaxRadius);
	//		}
	//		else if (InputManager::GetInstance().GetIsKeyPressed(KeyboardInput::Down))
	//		{
	//			MaxRadius -= step;
	//			if (MaxRadius <= 0.f)
	//			{
	//				MaxRadius = 0.f;
	//			}

	//			Scene.GetComponent<PointLight>(PointLightEntity).SetMaxRadius(MaxRadius);
	//		}
	//	});

	core.Run();

	return 0;
}
#endif