#pragma once

#include "../../EngineConstants.h"
#include "../MemoryUtils.h"
#include "../LowLevelLL/LowLevelLL.h"
// #include "../../LinkedList/LinkedList.h"

#ifdef max
#undef max
#endif

namespace Integrian3D::Memory
{
	class FreeListAllocator final
	{
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

		class Page final
		{
		public:
			explicit Page(const uint64_t size);
			~Page();

			Page(const Page& other) noexcept;
			Page(Page&& other) noexcept;
			Page& operator=(const Page& other) noexcept;
			Page& operator=(Page&& other) noexcept;

			void AddFreeHeader(FreeHeader* pFreeHeader);
			void RemoveFreeHeader(FreeHeader* pFreeHeader);
			__NODISCARD bool HasFreeHeader(FreeHeader* pFreeHeader) const;
			__NODISCARD FreeHeader* GetFreeHeader(const uint64_t index);

			void AddUsedHeader(AllocationHeader* pAllHeader);
			void RemoveUsedHeader(AllocationHeader* pAllHeader);
			__NODISCARD bool HasAllocationHeader(AllocationHeader* pAllHeader) const;

			void MergeFreeHeaders();

			__NODISCARD FreeHeader* FindFirstFreeHeader(const uint64_t size, const uint64_t alignment, uint64_t& padding) const;
			__NODISCARD FreeHeader* FindBestFreeHeader(const uint64_t size, const uint64_t alignment, uint64_t& padding) const;

			__NODISCARD uint64_t GetPageSize() const;
			__NODISCARD uint64_t GetNrOfFreeHeaders() const;
			__NODISCARD uint64_t GetNrOfUsedHeaders() const;

		private:
			void* m_pStart;
			LowLevelLL<FreeHeader*> m_FreeHeaders;
			LowLevelLL<AllocationHeader*> m_UsedHeaders;
			uint64_t m_PageSize;
		};

		inline constexpr static uint64_t m_NodePadding{ sizeof(FreeHeader) + sizeof(AllocationHeader) + sizeof(FreeHeader*) };

	public:
		FreeListAllocator(const PlacementPolicy policy = PlacementPolicy::FIND_FIRST_FIT);
		explicit FreeListAllocator(const uint64_t size, const PlacementPolicy policy = PlacementPolicy::FIND_FIRST_FIT);

		FreeListAllocator(const FreeListAllocator& other) noexcept;
		FreeListAllocator(FreeListAllocator&& other) noexcept;
		FreeListAllocator& operator=(const FreeListAllocator& other) noexcept;
		FreeListAllocator& operator=(FreeListAllocator&& other) noexcept;

		__NODISCARD void* Allocate(const uint64_t n, const uint64_t alignment);

		void Deallocate(void* p);

		template<typename T, typename ... Ts>
		constexpr T* Construct(T* p, Ts&&... args)
		{
			if (!p)
				return nullptr;

			return new (p) T{ __FORWARD(args)... };
		}

		template<typename T>
		constexpr void Destroy(T* p) const
		{
			if (!p)
				return;

			p->~T();
		}

		__NODISCARD void* Get(const uint64_t n) const;

		__NODISCARD uint64_t GetAllocationSize(const uint64_t n) const;

		__NODISCARD uint64_t Capacity() const;
		__NODISCARD uint64_t Size() const;

	private:
		FreeHeader* Find(const uint64_t size, const uint64_t alignment, uint64_t& padding, Page*& pPage);

		FreeHeader* FindFirst(const uint64_t size, const uint64_t alignment, uint64_t& padding, Page*& pPage);

		FreeHeader* FindBest(const uint64_t size, const uint64_t alignment, uint64_t& padding, Page*& pPage);

		__NODISCARD uint64_t CalculateNewCapacity(const uint64_t min) const;

		void Reallocate(const uint64_t requiredSize);

		PlacementPolicy m_PlacementPolicy;
		LowLevelLL<Page> m_Pages;
	};
}