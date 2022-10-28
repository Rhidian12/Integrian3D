// #define ENGINE
#ifdef ENGINE
#include "EngineConstants.h"
#include "Core/Core.h"
#include "Scene/Scene.h"
#include "SceneManager/SceneManager.h"
#include "Components/MeshComponent/MeshComponent.h"
#include "TextureManager/TextureManager.h"
#include "Components/TransformComponent/TransformComponent.h"
#include "Timer/Timer.h"

int main()
{
	using namespace Integrian3D;

	Core& core{ Core::CreateCore(1080,720) };

	Scene testScene{ "TestScene" };

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

	for (Entity i{}; i < vertices.size(); ++i)
	{
		indices.push_back(i);
	}

	TextureManager::GetInstance().AddTexture("__Wall", "Resources/wall.jpg");

	Entity entity = testScene.CreateEntity();
	testScene.AddComponent<MeshComponent>(entity, vertices, indices, TextureManager::GetInstance().GetTexture("__Wall"));
	TransformComponent& transform{ testScene.GetComponent<TransformComponent>(entity) };
	transform.Rotate(glm::vec3{ Math::ToRadians(-55.f), 0.f, 0.f });

	for (size_t i{}; i < 9; ++i)
	{
		Entity temp = testScene.CreateEntity();
		testScene.AddComponent<MeshComponent>(temp, vertices, indices, TextureManager::GetInstance().GetTexture("__Wall"));
		TransformComponent& transf{ testScene.GetComponent<TransformComponent>(temp) };
		transf.Translate(Math::RandomVec3(-5.f, 5.f));
		transform.Rotate(glm::vec3{ Math::ToRadians(-55.f), 0.f, 0.f });
	}

	testScene.AddUpdateCallback([](Scene& scene)->void
		{
			scene.CreateView<TransformComponent, MeshComponent>().ForEach([](TransformComponent& transform, const MeshComponent&)->void
				{
					transform.Rotate
					(
						glm::vec3
						{
							Math::ToRadians(25.f) * Timer::GetInstance().GetElapsedSeconds(),
							Math::ToRadians(50.f) * Timer::GetInstance().GetElapsedSeconds(),
							0.f
						}
					);
				});
		});

	SceneManager::GetInstance().AddScene(__MOVE(Scene, testScene));

	core.Run();
}
#else
#define CATCH_CONFIG_MAIN
#include "Libraries/Catch2/catch.hpp"
#include <vld.h>

#include "Array/Array.h"
TEST_CASE("Testing Basic Array of integers")
{
	using namespace Integrian3D;

	Array<int> arr{};

	REQUIRE(arr.Capacity() == 0);
	REQUIRE(arr.Size() == 0);

	SECTION("Adding 10 elements which causes several reallocations")
	{
		const int nrOfElements{ 10 };

		for (int i{}; i < nrOfElements; ++i)
		{
			arr.Add(i);
		}

		REQUIRE(arr.Size() == 10);
		REQUIRE(arr.Capacity() >= 10);
		REQUIRE(arr.Front() == 0);
		REQUIRE(arr.Back() == nrOfElements - 1);
		REQUIRE(arr[0] == 0);
		REQUIRE(arr[arr.Size() - 1] == nrOfElements - 1);
	}
}
#endif