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

	SECTION("Adding elements only through insertion")
	{
		for (int i{}; i < nrOfElements; ++i)
		{
			arr.Insert(i, i);
		}

		REQUIRE(arr[0] == 0);
		REQUIRE(arr.Size() == nrOfElements);
		REQUIRE(arr.Capacity() >= nrOfElements);
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
			arr.AddFront(i);
		}

		arr.AddFront(15);
		REQUIRE(arr.Size() == nrOfElements + 1);
		REQUIRE(arr[0] == 15);

		for (uint64_t i{ 1u }; i < arr.Size(); ++i)
			REQUIRE(arr[i] == nrOfElements - i);

		arr.AddFront(396);
		REQUIRE(arr.Size() == nrOfElements + 2);
		REQUIRE(arr[0] == 396);

		for (uint64_t i{ 2u }; i < arr.Size(); ++i)
			REQUIRE(arr[i] == nrOfElements - i + 1);
	}

	SECTION("Sorting an array using Insertion Sort (when array size < 64)")
	{
		std::initializer_list elems{ 5,0,3,6,7,15,356,-5 };
		std::vector<int> list{ elems };
		std::sort(list.begin(), list.end(), std::less<int>{});

		arr.AddRange(elems);

		REQUIRE(arr.Size() == list.size());

		arr.Sort();

		for (int i{}; i < arr.Size(); ++i)
			REQUIRE(arr[i] == list[i]);
	}


	SECTION("Sorting an array using Insertion Sort with specified predicate (when array size < 64)")
	{
		std::initializer_list elems{ 5,0,3,6,7,15,356,-5 };
		std::vector<int> list{ elems };
		std::sort(list.begin(), list.end(), std::less<int>{});

		arr.AddRange(elems);

		REQUIRE(arr.Size() == list.size());

		arr.Sort(std::less<int>{});

		for (int i{}; i < arr.Size(); ++i)
			REQUIRE(arr[i] == list[i]);
	}

	SECTION("Sorting an array using Merge Sort (when array size > 64)")
	{
		std::vector<int> list{};

		for (int i{ 100 }; i >= 0; --i)
		{
			list.push_back(i);
			arr.Add(i);
		}

		REQUIRE(arr.Size() == list.size());

		std::sort(list.begin(), list.end(), std::less<int>{});
		arr.Sort();
		std::sort(list.begin(), list.end());

		for (int i{}; i < arr.Size(); ++i)
			REQUIRE(arr[i] == list[i]);
	}

	SECTION("Sorting an array using Merge Sort with specified predicate (when array size > 64)")
	{
		std::vector<int> list{};

		for (int i{ 100 }; i >= 0; --i)
		{
			list.push_back(i);
			arr.Add(i);
		}

		REQUIRE(arr.Size() == list.size());

		std::sort(list.begin(), list.end(), std::less<int>{});
		arr.Sort(std::less<int>{});
		std::sort(list.begin(), list.end());

		for (int i{}; i < arr.Size(); ++i)
			REQUIRE(arr[i] == list[i]);
	}

	SECTION("Adding elements to the array using a C-array")
	{
		constexpr int size{ 8 };
		int newArr[size]{ 5,3,4,9,65,-15,-7,6 };

		arr.AddRange(newArr, size);

		for (int i{}; i < size; ++i)
			REQUIRE(arr[i] == newArr[i]);
	}
}

#include <string>
TEST_CASE("Testing Basic Array of characters")
{
	using namespace Integrian3D;

	TArray<char> arr{};

	SECTION("Adding characters")
	{
		const std::string letters{ "abcdefgh" };

		for (const char c : letters)
			arr.Add(c);

		for (size_t i{}; i < arr.Size(); ++i)
			REQUIRE(arr[i] == letters[i]);
	}

	SECTION("Adding Character to the front")
	{
		const std::string letters{ "abcdefgh" };

		for (const char c : letters)
			arr.AddFront(c);

		size_t counter{};
		for (int i{ static_cast<int>(arr.Size() - 1) }; i >= 0; --i)
			REQUIRE(letters[i] == arr[counter++]);
	}
}
#endif

#define TIMEPOINT_TESTS
#ifdef TIMEPOINT_TESTS
#include "Timer/Timer.h"
#include "Timer/Timepoint/Timepoint.h"
#include "Math/Math.h"
#include <type_traits>
TEST_CASE("Testing Basic Timepoints")
{
	using namespace Integrian3D::Math;
	using namespace Integrian3D::Time;

	constexpr double epsilon = 0.00001;

	Timepoint t1{}, t2{};

	REQUIRE(t1 == t2);
	REQUIRE(AreEqual((t1 + t2).Count(), 0.0, epsilon));
	REQUIRE(AreEqual((t2 + t1).Count(), 0.0, epsilon));
	REQUIRE(AreEqual((t1 - t2).Count(), 0.0, epsilon));
	REQUIRE(AreEqual((t2 - t1).Count(), 0.0, epsilon));

	t1 = Timepoint{ 5.0 };

	REQUIRE(t1 != t2);
	REQUIRE(AreEqual((t1 + t2).Count(), 5.0, epsilon));
	REQUIRE(AreEqual((t2 + t1).Count(), 5.0, epsilon));
	REQUIRE(AreEqual((t1 - t2).Count(), 5.0, epsilon));
	REQUIRE(AreEqual((t2 - t1).Count(), -5.0, epsilon));

	REQUIRE(std::is_same_v<uint64_t, decltype((t1 + t2).Count<TimeLength::Seconds, uint64_t>())>);

	REQUIRE(AreEqual((t1 + t2).Count<TimeLength::NanoSeconds>(), 5'000'000'000.0, epsilon));
	REQUIRE(AreEqual((t1 + t2).Count<TimeLength::MicroSeconds>(), 5'000'000.0, epsilon));
	REQUIRE(AreEqual((t1 + t2).Count<TimeLength::MilliSeconds>(), 5'000.0, epsilon));
	REQUIRE(AreEqual((t1 + t2).Count<TimeLength::Minutes>(), 0.083333, epsilon));
	REQUIRE(AreEqual((t1 + t2).Count<TimeLength::Hours>(), 0.001389, epsilon));

	t2 += t1;

	REQUIRE(AreEqual(t1.Count(), 5.0, epsilon));
	REQUIRE(AreEqual(t2.Count(), 5.0, epsilon));
	REQUIRE(t1 == t2);

	t1 -= t2;

	REQUIRE(AreEqual(t1.Count(), 0.0, epsilon));
	REQUIRE(AreEqual(t2.Count(), 5.0, epsilon));
	REQUIRE(t1 != t2);
}

TEST_CASE("Testing Timer")
{
	using namespace Integrian3D::Time;
	using namespace Integrian3D::Math;

	Timer& timer{ Timer::GetInstance() };
	REQUIRE(AreEqual(timer.GetElapsedSeconds(), 0.0));
	REQUIRE(!AreEqual(timer.Now().Count(), 0.0));

	timer.Start();
	REQUIRE(AreEqual(timer.GetElapsedSeconds(), 0.0));
	REQUIRE(!AreEqual(timer.Now().Count(), 0.0));

	Timepoint t1{ timer.Now() };

	timer.Update();
	REQUIRE(timer.Now() > t1);

	Timepoint t2{ timer.Now() };
	REQUIRE(t2 > t1);
	REQUIRE(t1 < t2);
}
#endif

#define SERIALIZING_BINARY_DATA_TESTS
#ifdef SERIALIZING_BINARY_DATA_TESTS
#include "IO/File/File.h"
#include "IO/Serializer/Serializer.h"
#include "Math/Math.h"

class NonPOD final
{
public:
	NonPOD()
		: m_A{}
		, m_B{}
	{}
	NonPOD(const int a, const double b)
		: m_A{ a }
		, m_B{ b }
	{}

	std::string Serialize() const
	{
		std::string data{};
		data.append(std::to_string(m_A) + ",");
		data.append(std::to_string(m_B));

		return data;
	}

	void Deserialize(std::string data)
	{
		m_A = std::stoi(data.substr(0, data.find_first_of(',')));
		data = data.substr(data.find_first_of(',') + 1);

		m_B = std::stod(data);
	}

	int GetA() const { return m_A; }
	double GetB() const { return m_B; }

private:
	int m_A;
	double m_B;
};

template<>
__NODISCARD __INLINE Integrian3D::TArray<char> Serialize<NonPOD>(const NonPOD& data)
{
	Integrian3D::TArray<char> arr{ Serialize(data.GetA()) };

	Integrian3D::TArray<char> arr2{ Serialize(data.GetB()) };

	arr.AddRange(arr2.cbegin(), arr2.cend());

	return arr;
}

template<>
__NODISCARD __INLINE NonPOD Deserialize<NonPOD>(const Integrian3D::TArray<char>& data)
{
	const int a{ Deserialize<int>(data) };
	const double b{ Deserialize<double>(data.SubArray(sizeof(int))) };

	return NonPOD{ a,b };
}

TEST_CASE("Testing Writing and Reading Binary files")
{
	using namespace Integrian3D::IO;
	using namespace Integrian3D::Math;

	SECTION("Testing Simple POD data")
	{
		{
			File file{ "Resources/TestBinaryFile.bin" };

			file.Write(Serialize(5));
			file.Write(Serialize(10.0));
			file.Write(Serialize(15.f));
			file.Write(Serialize('c'));
			file.Write(Serialize(36u), SeekMode::Start);

			const uint64_t constInt{ 951060u };
			file.Write(Serialize(constInt));

			file.Write();
		}

		{
			File file{ "Resources/TestBinaryFile.bin" };

			REQUIRE(Deserialize<unsigned int>(file.Read<unsigned int>()) == 36u);
			REQUIRE(Deserialize<int>(file.Read<int>()) == 5);
			REQUIRE(AreEqual(Deserialize<double>(file.Read<double>()), 10.0));
			REQUIRE(AreEqual(Deserialize<float>(file.Read<float>()), 15.f));
			REQUIRE(Deserialize<char>(file.Read<char>()) == 'c');
			REQUIRE(Deserialize<uint64_t>(file.Read<uint64_t>()) == 951060u);
		}
	}

	SECTION("Testing Custom POD data")
	{
		struct POD final
		{
			int A;
			float B;
			char C;
			double D;
			unsigned int E;
		};

		POD data{};
		data.A = 15;
		data.B = 36.f;
		data.C = 'w';
		data.D = -680.0;
		data.E = 68661u;

		{
			File file{ "Resources/TestBinaryFile.bin" };

			file.Write(Serialize(data));

			file.Write();
		}

		{
			File file{ "Resources/TestBinaryFile.bin" };

			const POD newData{ Deserialize<POD>(file.Read<POD>()) };

			REQUIRE(newData.A == data.A);
			REQUIRE(AreEqual(newData.B, data.B));
			REQUIRE(newData.C == data.C);
			REQUIRE(AreEqual(newData.D, data.D));
			REQUIRE(newData.E == data.E);
		}
	}

	SECTION("Testing custom non-POD data")
	{
		NonPOD data{ 974654, -651.64 };

		{
			File file{ "Resources/TestBinaryFile.bin" };

			file.Write(Serialize(data));

			file.Write();
		}

		{
			File file{ "Resources/TestBinaryFile.bin" };

			const NonPOD newData{ Deserialize<NonPOD>(file.Read<NonPOD>()) };

			REQUIRE(newData.GetA() == data.GetA());
			REQUIRE(AreEqual(newData.GetB(), data.GetB()));
		}
	}
}
#endif // SERIALIZING_BINARY_DATA_TESTS

// #define IASSET_READ_AND_WRITE_TESTS
#ifdef IASSET_READ_AND_WRITE_TESTS
#include "IO/Converters/TestConverter.h"
#include "IO/IAsset/IAssetWriter.h"
#include "IO/IAsset/IAssetReader.h"
#include "Math/Math.h"

TEST_CASE("Writing and Reading a .iasset file")
{
	using namespace Integrian3D::IO;
	using namespace Integrian3D::Math;

	SECTION("Testing POD data")
	{
		PODTestData data{};
		data.A = 15;
		data.B = 30.f;
		data.C = true;
		data.D = 60.0;
		data.E = 'F';

		SECTION("Writing a .iasset file")
		{
			IAssetWriter<TestConverter, PODTestData> writer{ "Resources/TestIAsset" };

			writer.Serialize(data);
		}

		SECTION("Reading a .iasset file")
		{
			IAssetReader<TestConverter, PODTestData> reader{ "Resources/TestIAsset" };

			PODTestData newData{};
			newData = reader.Deserialize();

			REQUIRE(data.A == newData.A);
			REQUIRE(AreEqual(data.B, newData.B));
			REQUIRE(data.C == newData.C);
			REQUIRE(AreEqual(data.D, newData.D));
			REQUIRE(data.E == newData.E);
		}
	}
}
#endif // IASSET_READ_AND_WRITE_TESTS

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
	return std::accumulate(arr.cbegin(), arr.cend(), 0.0) / static_cast<double>(arr.size());
}

#define ARRAY_BENCHMARKS
#ifdef ARRAY_BENCHMARKS
#include "Array/Array.h"
#include <deque>

#define ARR_CREATE
#ifdef ARR_CREATE
void BenchmarkCreation(const uint64_t iterations, const uint64_t nrOfOperationsPerIt)
{
	using namespace Integrian3D;
	using namespace Memory;

	using namespace Integrian3D;
	using namespace Memory;

	std::deque<double> standardArrTimes{};
	std::deque<double> vectorTimes{};

	for (uint64_t i{}; i < iterations; ++i)
	{
		standardArrTimes.push_back(Benchmark([nrOfOperationsPerIt]()->void
			{
				for (uint64_t i{}; i < nrOfOperationsPerIt; ++i)
				{
					TArray<uint64_t> standardArr{};
				}
			}));

		vectorTimes.push_back(Benchmark([nrOfOperationsPerIt]()->void
			{
				for (uint64_t i{}; i < nrOfOperationsPerIt; ++i)
				{
					std::vector<uint64_t> vec{};
				}
			}));
	}

	for (uint64_t i{}; i < iterations / 10u; ++i)
	{
		standardArrTimes.pop_back();
		standardArrTimes.pop_front();

		vectorTimes.pop_back();
		vectorTimes.pop_front();
	}

	std::cout << std::fixed << "Average Standard TArray creation time: " << GetAverage(standardArrTimes) << " seconds\n";
	std::cout << std::fixed << "Average std::vector creation time: " << GetAverage(vectorTimes) << " seconds\n";
}
#endif
#define ARR_PUSH_BACK
#ifdef ARR_PUSH_BACK
void BenchmarkPushBack(const uint64_t iterations, const uint64_t nrOfOperationsPerIt)
{
	using namespace Integrian3D;
	using namespace Memory;

	std::deque<double> standardArrTimes{};
	std::deque<double> customArrTimes{};
	std::deque<double> vectorTimes{};

	for (uint64_t i{}; i < iterations; ++i)
	{
		FreeListAllocator alloc{ 8192u };
		TArray<uint64_t> customArr{ __MOVE(alloc) };
		TArray<uint64_t> standardArr{};
		std::vector<uint64_t> vec{};

		standardArrTimes.push_back(Benchmark([&standardArr, nrOfOperationsPerIt]()->void
			{
				for (uint64_t i{}; i < nrOfOperationsPerIt; ++i)
				{
					standardArr.Add(i);
				}
			}));

		customArrTimes.push_back(Benchmark([&customArr, nrOfOperationsPerIt]()->void
			{
				for (uint64_t i{}; i < nrOfOperationsPerIt; ++i)
				{
					customArr.Add(i);
				}
			}));

		vectorTimes.push_back(Benchmark([&vec, nrOfOperationsPerIt]()->void
			{
				for (uint64_t i{}; i < nrOfOperationsPerIt; ++i)
				{
					vec.push_back(i);
				}
			}));
	}

	for (uint64_t i{}; i < iterations / 10u; ++i)
	{
		standardArrTimes.pop_back();
		standardArrTimes.pop_front();

		customArrTimes.pop_back();
		customArrTimes.pop_front();

		vectorTimes.pop_back();
		vectorTimes.pop_front();
	}

	std::cout << std::fixed << "Average Standard TArray::Add time: " << GetAverage(standardArrTimes) << " seconds\n";
	std::cout << std::fixed << "Average Custom Memory TArray::Add time: " << GetAverage(customArrTimes) << " seconds\n";
	std::cout << std::fixed << "Average std::vector::push_back time: " << GetAverage(vectorTimes) << " seconds\n";
}
#endif
#define ARR_INSERT
#ifdef ARR_INSERT
void BenchmarkInsert(const uint64_t iterations, const uint64_t nrOfOperationsPerIt)
{
	using namespace Integrian3D;
	using namespace Memory;

	std::deque<double> standardArrTimes{};
	std::deque<double> customArrTimes{};
	std::deque<double> vectorTimes{};

	for (uint64_t i{}; i < iterations; ++i)
	{
		FreeListAllocator alloc{ 8192u };
		TArray<uint64_t> customArr{ __MOVE(alloc) };
		TArray<uint64_t> standardArr{};
		std::vector<uint64_t> vec{};

		standardArrTimes.push_back(Benchmark([&standardArr, nrOfOperationsPerIt]()->void
			{
				for (uint64_t i{}; i < nrOfOperationsPerIt; ++i)
				{
					standardArr.Insert(i, i);
				}
			}));

		customArrTimes.push_back(Benchmark([&customArr, nrOfOperationsPerIt]()->void
			{
				for (uint64_t i{}; i < nrOfOperationsPerIt; ++i)
				{
					customArr.Insert(i, i);
				}
			}));

		vectorTimes.push_back(Benchmark([&vec, nrOfOperationsPerIt]()->void
			{
				for (uint64_t i{}; i < nrOfOperationsPerIt; ++i)
				{
					vec.insert(vec.cbegin() + i, i);
				}
			}));
	}

	for (uint64_t i{}; i < iterations / 10u; ++i)
	{
		standardArrTimes.pop_back();
		standardArrTimes.pop_front();

		customArrTimes.pop_back();
		customArrTimes.pop_front();

		vectorTimes.pop_back();
		vectorTimes.pop_front();
	}

	std::cout << std::fixed << "Average Standard TArray::Insert time: " << GetAverage(standardArrTimes) << " seconds\n";
	std::cout << std::fixed << "Average Custom Memory TArray::Insert time: " << GetAverage(customArrTimes) << " seconds\n";
	std::cout << std::fixed << "Average std::vector::insert time: " << GetAverage(vectorTimes) << " seconds\n";
}
#endif
#define ARR_ERASE_VALUE
#ifdef ARR_ERASE_VALUE
void BenchmarkEraseValue(const uint64_t iterations, const uint64_t nrOfOperationsPerIt)
{
	using namespace Integrian3D;
	using namespace Memory;

	std::deque<double> standardArrTimes{};
	std::deque<double> customArrTimes{};
	std::deque<double> vectorTimes{};

	for (uint64_t i{}; i < iterations; ++i)
	{
		FreeListAllocator alloc{ 8192u };
		TArray<uint64_t> customArr{ __MOVE(alloc) };
		TArray<uint64_t> standardArr{};
		std::vector<uint64_t> vec{};

		for (uint64_t j{}; j < nrOfOperationsPerIt; ++j)
		{
			customArr.Add(j);
			standardArr.Add(j);
			vec.push_back(j);
		}

		standardArrTimes.push_back(Benchmark([&standardArr, nrOfOperationsPerIt]()->void
			{
				for (uint64_t i{}; i < nrOfOperationsPerIt; ++i)
				{
					standardArr.Erase(i);
				}
			}));

		customArrTimes.push_back(Benchmark([&customArr, nrOfOperationsPerIt]()->void
			{
				for (uint64_t i{}; i < nrOfOperationsPerIt; ++i)
				{
					customArr.Erase(i);
				}
			}));

		vectorTimes.push_back(Benchmark([&vec, nrOfOperationsPerIt]()->void
			{
				for (uint64_t i{}; i < nrOfOperationsPerIt; ++i)
				{
					vec.erase(std::remove(vec.begin(), vec.end(), i), vec.end());
				}
			}));
	}

	for (uint64_t i{}; i < iterations / 10u; ++i)
	{
		standardArrTimes.pop_back();
		standardArrTimes.pop_front();

		customArrTimes.pop_back();
		customArrTimes.pop_front();

		vectorTimes.pop_back();
		vectorTimes.pop_front();
	}

	std::cout << std::fixed << "Average Standard TArray::Erase by value time: " << GetAverage(standardArrTimes) << " seconds\n";
	std::cout << std::fixed << "Average Custom Memory TArray::Erase by value time: " << GetAverage(customArrTimes) << " seconds\n";
	std::cout << std::fixed << "Average std::vector::Erase by value time: " << GetAverage(vectorTimes) << " seconds\n";
}
#endif
#define ARR_ERASE_IT
#ifdef ARR_ERASE_IT
void BenchmarkEraseIt(const uint64_t iterations, const uint64_t nrOfOperationsPerIt)
{
	using namespace Integrian3D;
	using namespace Memory;

	std::deque<double> standardArrTimes{};
	std::deque<double> customArrTimes{};
	std::deque<double> vectorTimes{};

	for (uint64_t i{}; i < iterations; ++i)
	{
		FreeListAllocator alloc{ 8192u };
		TArray<uint64_t> customArr{ __MOVE(alloc) };
		TArray<uint64_t> standardArr{};
		std::vector<uint64_t> vec{};

		for (uint64_t j{}; j < nrOfOperationsPerIt; ++j)
		{
			customArr.Add(j);
			standardArr.Add(j);
			vec.push_back(j);
		}

		standardArrTimes.push_back(Benchmark([&standardArr, nrOfOperationsPerIt]()->void
			{
				for (uint64_t i{}; i < nrOfOperationsPerIt; ++i)
				{
					standardArr.Erase(standardArr.begin());
				}
			}));

		customArrTimes.push_back(Benchmark([&customArr, nrOfOperationsPerIt]()->void
			{
				for (uint64_t i{}; i < nrOfOperationsPerIt; ++i)
				{
					customArr.Erase(customArr.begin());
				}
			}));

		vectorTimes.push_back(Benchmark([&vec, nrOfOperationsPerIt]()->void
			{
				for (uint64_t i{}; i < nrOfOperationsPerIt; ++i)
				{
					vec.erase(vec.begin());
				}
			}));
	}

	for (uint64_t i{}; i < iterations / 10u; ++i)
	{
		standardArrTimes.pop_back();
		standardArrTimes.pop_front();

		customArrTimes.pop_back();
		customArrTimes.pop_front();

		vectorTimes.pop_back();
		vectorTimes.pop_front();
	}

	std::cout << std::fixed << "Average Standard TArray::Erase by iterator time: " << GetAverage(standardArrTimes) << " seconds\n";
	std::cout << std::fixed << "Average Custom Memory TArray::Erase by iterator time: " << GetAverage(customArrTimes) << " seconds\n";
	std::cout << std::fixed << "Average std::vector::Erase by iterator time: " << GetAverage(vectorTimes) << " seconds\n";
}
#endif

int main()
{
	constexpr uint64_t iterations{ 1000u };
	constexpr uint64_t nrOfOperationsPerIt{ 1000u };

#ifdef _DEBUG
	std::cout << "Mode: Debug\n";
#else
	std::cout << "Mode: Release\n";
#endif
	std::cout << "Amount of iterations: " << iterations << "\n";
	std::cout << "Amount of operations per iteration: " << nrOfOperationsPerIt << "\n";

#ifdef ARR_CREATE
	BenchmarkCreation(iterations, nrOfOperationsPerIt);
	std::cout << "===============\n";
#endif
#ifdef ARR_PUSH_BACK
	BenchmarkPushBack(iterations, nrOfOperationsPerIt);
	std::cout << "===============\n";
#endif
#ifdef ARR_INSERT
	BenchmarkInsert(iterations, nrOfOperationsPerIt);
	std::cout << "===============\n";
#endif
#ifdef ARR_ERASE_VALUE
	BenchmarkEraseValue(iterations, nrOfOperationsPerIt);
	std::cout << "===============\n";
#endif
#ifdef ARR_ERASE_IT
	BenchmarkEraseIt(iterations, nrOfOperationsPerIt);
	std::cout << "===============\n";
#endif

	return 0;
}

#endif
#endif