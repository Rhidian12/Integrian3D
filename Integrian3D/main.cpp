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

#include "DebugUtility/DebugUtility.h"
#include "Array/Array.h"
TEST_CASE("Testing Basic Array of integers")
{
	using namespace Integrian3D;

	Array<int> arr{};

	REQUIRE(arr.Capacity() == 0);
	REQUIRE(arr.Size() == 0);
	REQUIRE(arr.Empty());

	const int nrOfElements{ 10 };

	SECTION("Adding 10 elements which causes several reallocations")
	{
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
		REQUIRE(arr.At(arr.Size() - 1) == nrOfElements - 1);
	}

	SECTION("Reserving and adding elements")
	{
		arr.Reserve(nrOfElements);

		REQUIRE(arr.Capacity() == 10);

		for (int i{}; i < nrOfElements; ++i)
		{
			arr.Add(i);
		}

		REQUIRE(arr.Size() == 10);
		REQUIRE(arr.Capacity() == 10);
		REQUIRE(arr.Front() == 0);
		REQUIRE(arr.Back() == nrOfElements - 1);
		REQUIRE(arr[0] == 0);
		REQUIRE(arr[arr.Size() - 1] == nrOfElements - 1);
	}

	SECTION("Clearing and removing elements")
	{
		for (int i{}; i < nrOfElements; ++i)
		{
			arr.Add(i);
		}

		arr.Pop();

		REQUIRE(arr.Size() == 9);
		REQUIRE(arr.Capacity() >= 10);

		for (size_t i{}; i < 5; ++i)
		{
			arr.Pop();
		}

		REQUIRE(arr.Size() == 4);
		REQUIRE(arr.Capacity() >= 10);

		arr.Clear();

		REQUIRE(arr.Size() == 0);
		REQUIRE(arr.Capacity() >= 10);
	}

	SECTION("Shrinking to size")
	{
		for (int i{}; i < nrOfElements; ++i)
		{
			arr.Add(i);
		}

		arr.Pop();
		arr.Pop();
		arr.Pop();

		arr.ShrinkToFit();

		REQUIRE(arr.Capacity() == arr.Size());
	}

	SECTION("Resizing array")
	{
		arr.Resize(nrOfElements);

		for (int i{}; i < nrOfElements; ++i)
		{
			REQUIRE(arr[i] == 0);
		}

		arr.Clear();

		arr.Resize(nrOfElements, 15);

		for (int i{}; i < nrOfElements; ++i)
		{
			REQUIRE(arr[i] == 15);
		}
	}

	SECTION("Inserting elements into the array")
	{
		for (int i{}; i < nrOfElements; ++i)
		{
			arr.Add(i);
		}

		arr.Insert(1, 15);

		REQUIRE(arr.Size() == nrOfElements + 1);
		REQUIRE(arr[1] == 15);
	}

	SECTION("Making an array with non-trivial destructor type")
	{
		class Special
		{
		public:
			~Special()
			{
				Debug::LogMessage("Getting destroyed", false);
			}
		};

		Array<Special> specialArr{};

		for (int i{}; i < nrOfElements; ++i)
		{
			specialArr.Add(Special{});
		}

		specialArr.Clear();
	}

	SECTION("Testing copy ctor")
	{
		for (int i{}; i < nrOfElements; ++i)
		{
			arr.Add(i);
		}

		Array<int> newArr{ arr };

		REQUIRE(newArr.Size() == arr.Size());
		REQUIRE(newArr.Capacity() == arr.Capacity());
		REQUIRE(newArr.Data() != arr.Data());

		for (int i{}; i < nrOfElements; ++i)
		{
			REQUIRE(newArr[i] == arr[i]);
		}
	}

	SECTION("Testing copy operator")
	{
		for (int i{}; i < nrOfElements; ++i)
		{
			arr.Add(i);
		}

		Array<int> newArr = arr;

		REQUIRE(newArr.Size() == arr.Size());
		REQUIRE(newArr.Capacity() == arr.Capacity());
		REQUIRE(newArr.Data() != arr.Data());

		for (int i{}; i < nrOfElements; ++i)
		{
			REQUIRE(newArr[i] == arr[i]);
		}
	}

	SECTION("Testing move ctor")
	{
		for (int i{}; i < nrOfElements; ++i)
		{
			arr.Add(i);
		}

		Array<int> newArr{ __MOVE(Array<int>, arr) };

		REQUIRE(arr.Size() == 0);
		REQUIRE(arr.Capacity() == 0);
		REQUIRE(arr.Empty());
		REQUIRE(arr.Data() == nullptr);

		REQUIRE(newArr.Size() == nrOfElements);
		REQUIRE(newArr.Capacity() >= nrOfElements);
		REQUIRE(!newArr.Empty());
		REQUIRE(newArr.Data() != nullptr);
	}

	SECTION("Testing move operator")
	{
		for (int i{}; i < nrOfElements; ++i)
		{
			arr.Add(i);
		}

		Array<int> newArr = __MOVE(Array<int>, arr);

		REQUIRE(arr.Size() == 0);
		REQUIRE(arr.Capacity() == 0);
		REQUIRE(arr.Empty());
		REQUIRE(arr.Data() == nullptr);

		REQUIRE(newArr.Size() == nrOfElements);
		REQUIRE(newArr.Capacity() >= nrOfElements);
		REQUIRE(!newArr.Empty());
		REQUIRE(newArr.Data() != nullptr);
	}
}
#endif