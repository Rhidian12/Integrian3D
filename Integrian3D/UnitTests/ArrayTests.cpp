#include <catch.hpp>

#include "../DebugUtility/DebugUtility.h"
#include "../Array/Array.h"

#include <string>

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
				LOG(Log, Debug, "Getting destroyed");
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

	SECTION("Testing EraseAll")
	{
		for (int i{}; i < 5; ++i)
		{
			arr.Add(i);
			arr.Add(7);
		}

		REQUIRE(arr.Size() == 10);
		REQUIRE(arr.Capacity() >= 10);

		arr.EraseAll(7);

		REQUIRE(arr.Size() == 5);
		REQUIRE(arr.Capacity() >= 10);

		arr.EraseAll([](const int i)->bool
			{
				return i > 1;
			});

		REQUIRE(arr.Size() == 2);
		REQUIRE(arr.Capacity() >= 10);
	}

	SECTION("Testing AddUnique")
	{
		for (int i{}; i < 10; ++i)
		{
			arr.Add(i);
		}

		REQUIRE(arr.Size() == 10);

		arr.AddUnique(-5);

		REQUIRE(arr.Size() == 11);

		arr.AddUnique(0);

		REQUIRE(arr.Size() == 11);
	}
}

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

TEST_CASE("Testing Array with Custom Structure")
{
	struct ArrayTestStruct final
	{
		ArrayTestStruct(const int Value)
		{
			Variable = new int{ Value };
		}
		~ArrayTestStruct()
		{
			if (Variable)
			{
				delete Variable;
				Variable = nullptr;
			}
		}
		ArrayTestStruct(const ArrayTestStruct& Other)
			: Variable{ new int{ *Other.Variable } }
		{}
		ArrayTestStruct(ArrayTestStruct&& Other)
			: Variable{ __MOVE(Other.Variable) }
		{
			Other.Variable = nullptr;
		}
		ArrayTestStruct& operator=(const ArrayTestStruct& Other)
		{
			if (Variable)
			{
				delete Variable;
				Variable = nullptr;
			}

			Variable = new int{ *Other.Variable };

			return *this;
		}
		ArrayTestStruct& operator=(ArrayTestStruct&& Other)
		{
			if (Variable)
			{
				delete Variable;
				Variable = nullptr;
			}

			Variable = __MOVE(Other.Variable);
			Other.Variable = nullptr;

			return *this;
		}

		int* Variable;
	};

	using namespace Integrian3D;
	TArray<ArrayTestStruct> Array{};
	constexpr static int NrOfElements = 10;

	SECTION("Creating Basic Array of Custom Structure")
	{
		REQUIRE(Array.Size() == 0);
		REQUIRE(Array.Capacity() == 0);

		for (int i{}; i < NrOfElements; ++i)
		{
			Array.Add(ArrayTestStruct{ i });
		}

		REQUIRE(Array.Size() == NrOfElements);
		REQUIRE(Array.Capacity() >= NrOfElements);
	}
}