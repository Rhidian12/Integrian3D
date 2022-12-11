// #define ENGINE
#define TESTS
// #define BENCHMARKS
#ifdef ENGINE
#include "EngineConstants.h"
#include "Core/Core.h"
#include "Scene/Scene.h"
#include "SceneManager/SceneManager.h"
#include "Components/MeshComponent/MeshComponent.h"
#include "TextureManager/TextureManager.h"
#include "Components/TransformComponent/TransformComponent.h"
#include "Timer/Timer.h"
#include "Memory/Allocator/Allocator.h"
#include "Memory/LinearAllocator/LinearAllocator.h"

int main()
{
	using namespace Integrian3D;
	using namespace Integrian3D::Memory;

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
#elif defined TESTS
#define CATCH_CONFIG_MAIN
#include "Libraries/Catch2/catch.hpp"
#include <vld.h>

#define ALLOCATOR_TESTS
#ifdef ALLOCATOR_TESTS
#include "Memory/Allocator/Allocator.h"
#include "Memory/FreeListAllocator/FreeListAllocator.h"

TEST_CASE("Testing the Allocator Interface")
{
	using namespace Integrian3D::Memory;

	FreeListAllocator alloc{ 36 };
	Allocator<FreeListAllocator> allocInterface{ std::move(alloc) };

	auto& a = allocInterface.Allocate<int>(1);
	 *a = 5;
	 REQUIRE(*a == 5);

	auto& b = allocInterface.Allocate<int>(1);
	*b = 15;
	REQUIRE(*a == 5);
	REQUIRE(*b == 15);

	auto& c = allocInterface.Allocate<int>(50);
	REQUIRE(*a == 5);
	REQUIRE(*b == 15);

	auto& d = allocInterface.Allocate<int>(1);
	*d = 25;
	REQUIRE(*a == 5);
	REQUIRE(*b == 15);
	REQUIRE(*d == 25);

	allocInterface.Deallocate(c);
	REQUIRE(*a == 5);
	REQUIRE(*b == 15);
	REQUIRE(*d == 25);

	auto& e = allocInterface.Allocate<double>(1);
	*e = 36.0;
	REQUIRE(*a == 5);
	REQUIRE(*b == 15);
	REQUIRE(*d == 25);
	REQUIRE(*e == 36.0);

	auto& f = allocInterface.Allocate<int>(50);
	REQUIRE(*a == 5);
	REQUIRE(*b == 15);
	REQUIRE(*d == 25);
	REQUIRE(*e == 36.0);

	int* pStart{ &f };

	REQUIRE(&f == pStart);

	for (int i{}; i < 50; ++i)
		*(pStart + i) = i;

	for (int i{}; i < 50; ++i)
		REQUIRE(pStart[i] == i);

#pragma warning ( push )
#pragma warning ( disable : 4189 ) /* warning C4189: 'g': local variable is initialized but not referenced */
	auto& g = allocInterface.Allocate<double>(10);
#pragma warning ( pop )
	allocInterface.Deallocate(f);
}
#endif

#define ARRAY_TESTS
#ifdef ARRAY_TESTS
#include "DebugUtility/DebugUtility.h"
#include "Array/Array.h"
TEST_CASE("Testing Basic Array of integers")
{
	using namespace Integrian3D;

	TArray<int> arr{};

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

		arr[0] = 15;
		REQUIRE(arr.Front() == 15);
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

		TArray<Special> specialArr{};

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

		TArray<int> newArr{ arr };

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

		TArray<int> newArr = arr;

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

		TArray<int> newArr{ __MOVE(arr) };

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

		TArray<int> newArr = __MOVE(arr);

		REQUIRE(arr.Size() == 0);
		REQUIRE(arr.Capacity() == 0);
		REQUIRE(arr.Empty());
		REQUIRE(arr.Data() == nullptr);

		REQUIRE(newArr.Size() == nrOfElements);
		REQUIRE(newArr.Capacity() >= nrOfElements);
		REQUIRE(!newArr.Empty());
		REQUIRE(newArr.Data() != nullptr);
	}

	SECTION("Comparing if two arrays are equal")
	{
		for (int i{}; i < nrOfElements; ++i)
		{
			arr.Add(i);
		}

		TArray<int> newArr{ arr };
		REQUIRE(arr == newArr);

		newArr.Pop();
		REQUIRE(arr != newArr);

		arr.Pop();
		REQUIRE(arr == newArr);

		arr.Back() = 65;
		REQUIRE(arr != newArr);
	}

	SECTION("Selecting a range of an array")
	{
		for (int i{}; i < nrOfElements; ++i)
		{
			arr.Add(i);
		}

		TArray<int> newArr{ arr.Select([](const int& a)->bool
			{
				return a > 5;
			}) };

		for (size_t i{}; i < newArr.Size(); ++i)
		{
			REQUIRE(newArr[i] > 5);
		}
	}

	SECTION("Create a vector with a start size")
	{
		TArray<int> newArr{ 10_size, 15 };

		REQUIRE(newArr.Size() == 10);
		REQUIRE(newArr.Capacity() >= 10);

		for (size_t i{}; i < newArr.Size(); ++i)
		{
			REQUIRE(newArr[i] == 15);
		}
	}

	SECTION("Create a vector with a start capacity")
	{
		TArray<int> newArr{ 10_capacity };

		REQUIRE(newArr.Size() == 0);
		REQUIRE(newArr.Capacity() == 10);
		REQUIRE(newArr.Empty());
	}

	SECTION("Using iterators on the array")
	{
		for (int i{}; i < nrOfElements; ++i)
		{
			arr.Add(i);
		}

		int counter{};
		for (int elem : arr)
		{
			REQUIRE(elem == counter++);
		}

		arr.Clear();

		for (int elem : arr)
		{
			elem;
			REQUIRE(false);
		}
	}

	SECTION("Initialize array using iterators")
	{
		for (int i{}; i < nrOfElements; ++i)
		{
			arr.Add(i);
		}

		TArray<int> newArr{ arr.begin(), arr.end() };

		REQUIRE(newArr.Size() == arr.Size());

		int counter{};
		for (const int elem : newArr)
		{
			REQUIRE(elem == arr[counter++]);
		}
	}

	SECTION("Add a range to an array")
	{
		arr.AddRange({ 0,1,2,3,4,5 });

		REQUIRE(arr.Size() == 6);
		REQUIRE(arr.Capacity() >= 6);
		REQUIRE(arr.At(0) == 0);
		REQUIRE(arr.At(5) == 5);

		TArray<int> newArr{};

		newArr.AddRange(arr.begin(), arr.Find(4));

		REQUIRE(newArr.Back() == 3);
		REQUIRE(newArr.Size() == arr.Size() - 2);

		for (size_t i{}; i < newArr.Size(); ++i)
		{
			REQUIRE(newArr[i] == arr[i]);
		}

		newArr.Clear();

		newArr.AddRange(arr.begin(), arr.end());

		REQUIRE(newArr.Back() == 5);
		REQUIRE(newArr.Size() == arr.Size());

		for (size_t i{}; i < newArr.Size(); ++i)
		{
			REQUIRE(newArr[i] == arr[i]);
		}
	}

	SECTION("Find an element in the array")
	{
		for (int i{}; i < nrOfElements; ++i)
		{
			arr.Add(i);
		}

		TArray<int>::It it{ arr.Find(5) };

		REQUIRE(it != arr.end());

		it = arr.Find(-1);

		REQUIRE(it == arr.end());

		it = arr.Find([](const int a)->bool
			{
				return a == 6;
			});

		REQUIRE(it != arr.end());
	}

	SECTION("Finding all elements in the array")
	{
		for (int i{}; i < 5; ++i)
		{
			arr.Add(5);
		}
		for (int i{}; i < 5; ++i)
		{
			arr.Add(i);
		}

		TArray<int> newArr{ arr.FindAll(5) };

		REQUIRE(newArr.Size() == 5);

		newArr = arr.FindAll(-1);

		REQUIRE(newArr.Size() == 0);
	}

	SECTION("Erasing elements in the array")
	{
		for (int i{}; i < nrOfElements; ++i)
		{
			arr.Add(i);
		}

		arr.Erase(3);

		REQUIRE(arr.Size() == nrOfElements - 1);
		REQUIRE(arr.Find(3) == arr.end());

		int counter{};
		for (int i{}; i < arr.Size(); ++i)
		{
			REQUIRE(arr[i] == counter++);

			if (counter == 3)
				++counter;
		}

		arr.Erase(arr.begin());
		REQUIRE(arr.Size() == nrOfElements - 2);
		REQUIRE(arr[0] == 1);
	}

	SECTION("Erasing a range of elements in the array")
	{
		for (int i{}; i < nrOfElements; ++i)
		{
			arr.Add(i);
		}

		arr.EraseRange(arr.Find(2), arr.Find(9));

		REQUIRE(arr.Size() == 2);
		REQUIRE(arr.Front() == 0);
		REQUIRE(arr.Back() == 1);

		arr.Clear();

		for (int i{}; i < nrOfElements; ++i)
		{
			arr.Add(i);
		}

		arr.EraseRange(3, 15); // should not crash

		REQUIRE(arr.Size() == 3);
	}

	SECTION("Popping off the front of the array")
	{
		for (int i{}; i < nrOfElements; ++i)
		{
			arr.Add(i);
		}

		arr.PopFront();
		REQUIRE(arr.Size() == nrOfElements - 1);
		REQUIRE(arr.Front() == 1);

		arr.PopFront();
		REQUIRE(arr.Size() == nrOfElements - 2);
		REQUIRE(arr.Front() == 2);
	}

	SECTION("Adding to the front of the array")
	{
		for (int i{}; i < nrOfElements; ++i)
		{
			arr.Add(i);
		}

		arr.AddFront(15);
		REQUIRE(arr.Size() == nrOfElements + 1);
		REQUIRE(arr[0] == 15);

		arr.AddFront(396);
		REQUIRE(arr.Size() == nrOfElements + 2);
		REQUIRE(arr[0] == 396);
	}
}
#endif

//#define STACK_ALLOCATOR_TESTS
#ifdef STACK_ALLOCATOR_TESTS
#include "Memory/StackAllocator/StackAllocator.h"

TEST_CASE("Testing the stack allocator")
{
	using namespace Integrian3D::Memory;

	constexpr int size{ 512 };

	StackAllocator<size> alloc{};

	REQUIRE(alloc.Size() == 0);
	REQUIRE(alloc.Capacity() == size);
	REQUIRE(alloc.MaxSize() == size);
	REQUIRE(alloc.Data() != nullptr);

	int* test{ alloc.Allocate<int>(1) };

	REQUIRE(test != nullptr);
	REQUIRE(alloc.Size() >= sizeof(int));
	REQUIRE(alloc.Capacity() == size);
	REQUIRE(alloc.MaxSize() == size);

	alloc.Deallocate(test, 1);

	REQUIRE(alloc.Size() == 0);

	test = alloc.Allocate<int>(size / sizeof(int));

	REQUIRE(test != nullptr);
	REQUIRE(alloc.Size() == alloc.MaxSize());

	alloc.Deallocate(test, size / sizeof(int));

	REQUIRE(alloc.Size() == 0);

	test = alloc.Allocate<int>(1);
	int* test2 = alloc.Allocate<int>(1);

	REQUIRE(test != test2);
	REQUIRE(alloc.Size() >= sizeof(int) * 2);
}
#endif

//#define LINEAR_ALLOCATOR_TESTS
#ifdef LINEAR_ALLOCATOR_TESTS
#include "Memory/LinearAllocator/LinearAllocator.h"

TEST_CASE("Testing the Linear Allocator")
{
	using namespace Integrian3D::Memory;

	constexpr int size{ 512 };

	LinearAllocator alloc{ size };

	REQUIRE(alloc.Size() == 0);
	REQUIRE(alloc.Capacity() == size);
	REQUIRE(alloc.MaxSize() == size);
	REQUIRE(alloc.Data() != nullptr);

	int* test{ alloc.Allocate<int>(1) };

	REQUIRE(test != nullptr);
	REQUIRE(alloc.Size() >= sizeof(int));
	REQUIRE(alloc.Capacity() == size);
	REQUIRE(alloc.MaxSize() == size);

	test = alloc.Allocate<int>(size / sizeof(int) - 1);

	REQUIRE(test != nullptr);
	REQUIRE(alloc.Size() == alloc.MaxSize());
}
#endif

// #define FREELIST_ALLOCATOR_TESTS
#ifdef FREELIST_ALLOCATOR_TESTS
#include "Memory/FreeListAllocator/FreeListAllocator.h"

TEST_CASE("Testing the Free List Allocator")
{
	using namespace Integrian3D::Memory;

	SECTION("Basic functionality")
	{
		FreeListAllocator alloc{};

		REQUIRE(alloc.Size() == 0);
		REQUIRE(alloc.Capacity() >= 0);
		REQUIRE(alloc.Data() != nullptr);

		int* test{ alloc.Allocate<int>(1) };

		REQUIRE(test != nullptr);
		REQUIRE(alloc.Size() == 1);

		test = alloc.Allocate<int>(100);

		REQUIRE(test != nullptr);
		REQUIRE(alloc.Size() == 2);

		alloc.Deallocate(test);

		REQUIRE(alloc.Size() == 1);
	}

	SECTION("Testing reallocation")
	{
		FreeListAllocator alloc{ 40 };

		REQUIRE(alloc.Capacity() == 40);
		REQUIRE(alloc.Size() == 0);

		int* test = alloc.Allocate<int>(1); // should be fine

		REQUIRE(test != nullptr);
		REQUIRE(alloc.Size() == 1);

		int* test2 = alloc.Allocate<int>(1); // should fire a reallocation

		REQUIRE(test2 != nullptr);
		REQUIRE(alloc.Size() == 2);
		REQUIRE(alloc.Capacity() == 77); // hardcoded w/e i didnt ask

		alloc.Deallocate(test2);
		test2 = nullptr;

		REQUIRE(alloc.Size() == 1);

		test2 = alloc.Allocate<int>(1);

		REQUIRE(test2 != nullptr);
		REQUIRE(alloc.Size() == 2);
		REQUIRE(alloc.Capacity() == 77); // hardcoded w/e i didnt ask

		alloc.Deallocate(test);
		test = nullptr;

		REQUIRE(alloc.Size() == 1);

		test = alloc.Allocate<int>(1);
		REQUIRE(test != nullptr);
		REQUIRE(alloc.Size() == 2);
		REQUIRE(alloc.Capacity() == 77); // hardcoded w/e i didnt ask
	}
}
#endif
#elif defined BENCHMARKS
#include <chrono>
#include <numeric>
#include <deque>
#include <iostream>

template<typename Fn>
double Benchmark(Fn&& fn)
{
	using Clock = std::chrono::high_resolution_clock;

	Clock::time_point t1{ Clock::now() };

	fn();

	Clock::time_point t2{ Clock::now() };

	return std::chrono::duration<double>{t2 - t1}.count();
}

double GetAverage(const std::deque<double>& arr)
{
	return std::accumulate(arr.cbegin(), arr.cend(), 0.0);
}

#define ARRAY_BENCHMARKS
#ifdef ARRAY_BENCHMARKS
#include "Array/Array.h"
#include <deque>

int main()
{
	using namespace Integrian3D;
	using namespace Memory;

	constexpr uint64_t iterations{ 100'000u };
	std::deque<double> arrTimes{};
	std::deque<double> vectorTimes{};

	constexpr uint64_t amountOfPushbacks{ 1000u };

	for (uint64_t i{}; i < iterations; ++i)
	{
		arrTimes.push_back(Benchmark([amountOfPushbacks]()->void
			{
				LinearAllocator alloc{ sizeof(uint64_t) * amountOfPushbacks };
				Array<uint64_t, LinearAllocator> arr{ alloc };
				for (uint64_t i{}; i < amountOfPushbacks; ++i)
				{
					arr.Add(i);
				}
			}));

		vectorTimes.push_back(Benchmark([amountOfPushbacks]()->void
			{
				std::vector<uint64_t> vec{};
				for (uint64_t i{}; i < amountOfPushbacks; ++i)
				{
					vec.push_back(i);
				}
			}));
	}

	for (uint64_t i{}; i < iterations / 10u; ++i)
	{
		arrTimes.pop_back();
		arrTimes.pop_front();

		vectorTimes.pop_back();
		vectorTimes.pop_front();
	}

#ifdef _DEBUG
	std::cout << "Mode: Debug\n";
#else
	std::cout << "Mode: Release\n";
#endif
	std::cout << "Amount of iterations: " << iterations << "\n";
	std::cout << "Amount of push_backs: " << amountOfPushbacks << "\n";
	std::cout << "Array time: " << GetAverage(arrTimes) << "\n";
	std::cout << "Array time: " << GetAverage(vectorTimes) << "\n";
}
#endif
#endif