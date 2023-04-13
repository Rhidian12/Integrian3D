#if __has_include(<vld.h>)
#	include <vld.h>
#endif

#define ENGINE
// #define UNIT_TESTS

#ifdef ENGINE

int RunEngine(int argc, char* argv[]);

#elif defined UNIT_TESTS

int RunUnitTests(int argc, char* argv[]);

#endif

int main(int argc, char* argv[])
{
#ifdef ENGINE

	return RunEngine(argc, argv);

#elif defined UNIT_TESTS

	return RunUnitTests(argc, argv);

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

int main()
{
	using namespace Integrian3D;
	using namespace Integrian3D::Memory;

	Core& core{ Core::CreateCore(1080,720) };

	std::vector<Vertex> vertices =
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

	std::vector<uint32_t> indices{};

	for (uint32_t i{}; i < vertices.size(); ++i)
	{
		indices.push_back(i);
	}

	TextureManager::GetInstance().AddTexture("__Wall", "Resources/wall.jpg");

	Scene* pTestScene{ new Scene{ "TestScene" } };
	SceneManager::GetInstance().AddScene(pTestScene);

	GameObject* pEntity{ Instantiate() };
	pEntity->AddComponent(new TestRotateComponent{ pEntity });
	pEntity->AddComponent(new MeshComponent{ pEntity, vertices, indices, TextureManager::GetInstance().GetTexture("__Wall") });
	pEntity->pTransform->Rotate(Math::Vec3D{ Math::ToRadians(-55.f), 0.f, 0.f });

	for (size_t i{}; i < 9; ++i)
	{
		GameObject* pTemp{ Instantiate() };
		pTemp->AddComponent(new TestRotateComponent{ pTemp });
		pTemp->AddComponent(new MeshComponent{ pTemp, vertices, indices, TextureManager::GetInstance().GetTexture("__Wall") });
		pTemp->pTransform->Translate(Math::RandomVec3D(-5.f, 5.f));
		pTemp->pTransform->Rotate(Math::Vec3D{ Math::ToRadians(-55.f), 0.f, 0.f });
	}

	core.Run();
}
#endif