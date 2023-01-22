#pragma once

#include "../../EngineConstants.h"
#include "../MemoryUtils.h"
// #include "../../LinkedList/LinkedList.h"

#ifdef max
#undef max
#endif

namespace Integrian3D::Memory
{
	class FreeListAllocator final
	{
		template<typename T>
		struct Node final
		{
			Node(const T& data)
				: Data{ data }
				, pNext{}
			{}

			T Data;
			Node* pNext;
		};

	public:
		enum class PlacementPolicy
		{
			FIND_FIRST_FIT,
			FIND_BEST_FIT
		};

	private:
		/* [CRINGE]: Do we really need 2 seperate structs for this kind of stuff? Could we not re-use the FreeHeader? */
		struct FreeHeader final
		{
			uint64_t Size;
		};

		/* [CRINGE]: Is this Padding necessary? Could we not just keep track of the entire Size instead of splitting it up? */
		struct AllocationHeader final
		{
			uint64_t Size;
			uint64_t Padding;
		};

		inline constexpr static uint64_t m_NodePadding{ sizeof(FreeHeader) + sizeof(AllocationHeader) + sizeof(FreeHeader*) };

	public:
		FreeListAllocator(const PlacementPolicy policy = PlacementPolicy::FIND_FIRST_FIT);
		explicit FreeListAllocator(const uint64_t size, const PlacementPolicy policy = PlacementPolicy::FIND_FIRST_FIT);

		~FreeListAllocator();

		FreeListAllocator(const FreeListAllocator& other) noexcept;
		FreeListAllocator(FreeListAllocator&& other) noexcept;
		FreeListAllocator& operator=(const FreeListAllocator& other) noexcept;
		FreeListAllocator& operator=(FreeListAllocator&& other) noexcept;

		__NODISCARD void* Allocate(const uint64_t n, const uint64_t alignment);

		void Deallocate(void* p);

		template<typename T>
		constexpr void Destroy(T* p) const
		{
			if (!p)
				return;

			p->~T();
		}

		__NODISCARD void* Get(const uint64_t n) const;

		__NODISCARD uint64_t GetAllocationSize(const uint64_t n) const;

		__NODISCARD constexpr uint64_t Capacity() const
		{
			return m_Capacity;
		}
		__NODISCARD constexpr uint64_t Size() const
		{
			return m_Size;
		}
		__NODISCARD constexpr uint64_t MaxSize() const
		{
			return std::numeric_limits<uint64_t>::max();
		}
		__NODISCARD constexpr void* Data() const
		{
			return m_pStart;
		}

	private:
		constexpr void Find(const uint64_t size, const uint64_t alignment, uint64_t& padding, FreeHeader*& pFreeHeader)
		{
			switch (m_PlacementPolicy)
			{
				case PlacementPolicy::FIND_FIRST_FIT:
					FindFirst(size, alignment, padding, pFreeHeader);
					break;
				case PlacementPolicy::FIND_BEST_FIT:
					FindBest(size, alignment, padding, pFreeHeader);
					break;
			}
		}

		void FindFirst(const uint64_t size, const uint64_t alignment, uint64_t& padding, FreeHeader*& pFreeHeader);

		void FindBest(const uint64_t size, const uint64_t alignment, uint64_t& padding, FreeHeader*& pFreeHeader);

		__NODISCARD constexpr uint64_t CalculateNewCapacity(const uint64_t min) const
		{
			const uint64_t oldCap{ Capacity() };
			const uint64_t maxCap{ MaxSize() };

			if (oldCap > maxCap - oldCap / 2u)
			{
				return maxCap;
			}

			const uint64_t newCap{ oldCap + oldCap / 2u };

			// If our growth is insufficient, return just the bare minimum
			if (newCap < min)
			{
				return min;
			}

			return newCap;
		}

		void Reallocate(const uint64_t requiredSize);

	#pragma region Linked_List
	
		template<typename T>
		void Add(Node<T>*& pList, const T& data)
		{
			const uint64_t oldSize{ Count(pList) };
			Node<T>* pNewList{ static_cast<Node<T>*>(malloc(sizeof(Node<T>) * (oldSize + 1))) };
			pNewList->pNext = nullptr;
			pNewList->Data = nullptr;

			if (pList != nullptr)
			{
				pNewList->Data = __MOVE(pList->Data);

				Node<T>* pPreviousNode{ pNewList };

				for (uint64_t i{ 1 }; i < oldSize; ++i)
				{
					Node<T>* pNewNode{ reinterpret_cast<Node<T>*>(reinterpret_cast<uint64_t>(pNewList) + sizeof(Node<T>) * i) };
					pNewNode->pNext = nullptr;
					pNewNode->Data = __MOVE(Get(pList, i)->Data);

					pPreviousNode->pNext = pNewNode;
					pPreviousNode = pNewNode;
				}

				Node<T>* pNewNode{ reinterpret_cast<Node<T>*>(reinterpret_cast<uint64_t>(pNewList) + sizeof(Node<T>) * oldSize) };
				pNewNode->pNext = nullptr;
				pNewNode->Data = data;

				Get(pNewList, Count(pNewList) - 1)->pNext = pNewNode;

				free(pList);
			}
			else
			{
				pNewList->Data = data;
			}

			pList = pNewList;
		}

		template<typename T>
		void Remove(Node<T>*& pList, const T& data)
		{
			__ASSERT(pList != nullptr);

			const uint64_t oldSize{ Count(pList) };

			Node<T>* pNewList{ static_cast<Node<T>*>(malloc(sizeof(Node<T>) * (oldSize - 1))) };
			pNewList->pNext = nullptr;
			pNewList->Data = nullptr;

			Node<T>* pPreviousNode{};

			for (uint64_t i{}, newListCounter{}; i < oldSize; ++i)
			{
				if ((pList + i)->Data == data)
					continue;

				Node<T>* pNewNode{ reinterpret_cast<Node<T>*>(reinterpret_cast<uint64_t>(pNewList) + sizeof(Node<T>) * newListCounter) };
				pNewNode->pNext = nullptr;
				pNewNode->Data = __MOVE(Get(pList, i)->Data);

				if (pPreviousNode)
					pPreviousNode->pNext = pNewNode;

				pPreviousNode = pNewNode;

				++newListCounter;
			}

			free(pList);
			pList = pNewList;

			if (!pPreviousNode)
				pList = nullptr;
		}

		template<typename T>
		__NODISCARD Node<T>* Get(Node<T>* pList, const uint64_t index) const
		{
			__ASSERT(pList != nullptr);
			__ASSERT(index < Count(pList));

			Node<T>* pNode{ pList };

			for (uint64_t i{}; i < index; ++i)
				pNode = pNode->pNext;

			return pNode;
		}

		template<typename T>
		__NODISCARD uint64_t Count(const Node<T>* pList) const
		{
			uint64_t count{};

			while (pList)
			{
				++count;

				pList = pList->pNext;
			}

			return count;
		}
	#pragma endregion

		void* m_pStart;
		PlacementPolicy m_PlacementPolicy;
		// LinkedList<FreeHeader*> m_FreeList;
		// LinkedList<AllocationHeader*> m_UsedList;
		Node<FreeHeader*>* m_FreeList;
		Node<AllocationHeader*>* m_UsedList;
		uint64_t m_Size;
		uint64_t m_Capacity;
	};
}