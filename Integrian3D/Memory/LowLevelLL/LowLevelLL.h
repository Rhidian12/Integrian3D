#pragma once

#include "../../EngineConstants.h"

namespace Integrian3D::Memory
{
	template<typename T>
	class LowLevelLL final
	{
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

#pragma region Ctor_Dtor

		LowLevelLL()
			: m_pHead{}
		{}
		~LowLevelLL()
		{
			Clear();
		}

#pragma endregion

#pragma region RuleOf5

		LowLevelLL(const LowLevelLL& other) noexcept
			: m_pHead{}
		{
			const uint64_t size{ other.Count() };
			Node* pPrevious{ m_pHead };

			for (uint64_t i{}; i < size; ++i)
			{
				Node* const pCurrent{ reinterpret_cast<Node*>(malloc(sizeof(Node))) };

				new (pCurrent) Node{ other.Get(i) };

				if (!m_pHead)
					m_pHead = pCurrent;

				if (pPrevious)
					pPrevious->pNext = pCurrent;
				else
					pPrevious = pCurrent;
			}
		}
		LowLevelLL(LowLevelLL&& other) noexcept
			: m_pHead{ __MOVE(other.m_pHead) }
		{
			other.m_pHead = nullptr;
		}
		LowLevelLL& operator=(const LowLevelLL& other) noexcept
		{
			if (m_pHead)
				Clear();

			const uint64_t size{ other.Count() };
			Node* pPrevious{ m_pHead };

			for (uint64_t i{}; i < size; ++i)
			{
				Node* const pCurrent{ reinterpret_cast<Node*>(malloc(sizeof(Node))) };

				new (pCurrent) Node{ other.Get(i) };

				if (!m_pHead)
					m_pHead = pCurrent;

				if (pPrevious)
					pPrevious->pNext = pCurrent;
				else
					pPrevious = pCurrent;
			}

			return *this;
		}
		LowLevelLL& operator=(LowLevelLL&& other) noexcept
		{
			if (m_pHead)
				Clear();

			m_pHead = __MOVE(other.m_pHead);
			other.m_pHead = nullptr;

			return *this;
		}

#pragma endregion

#pragma region Add_Remove

		T& Add(const T& data)
		{
			Node* const pLast{ GetLastElement() };

			Node* const pNewNode{ reinterpret_cast<Node*>(malloc(sizeof(Node))) };
			new (pNewNode) Node{ data };

			pLast->pNext = pNewNode;

			return pNewNode->Data;
		}

		void Remove(const T& value)
		{
			Node* pPrevious{};
			Node* pCurrent{ m_pHead };
			const uint64_t size{ Count() };
			for (uint64_t i{}; i < size; ++i)
			{
				if (!pCurrent)
					return;

				if (pCurrent->Data == value)
				{
					/* relink */
					if (!pPrevious)
						m_pHead = pCurrent->pNext;
					else
						pPrevious->pNext = pCurrent->pNext;

					Node* pNext{ pCurrent->pNext };

					/* delete old node */
					free(pCurrent);
					pCurrent = nullptr;

					pCurrent = pNext;

					continue;
				}

				pPrevious = pCurrent;
				pCurrent = pCurrent->pNext;
			}
		}

#pragma endregion

#pragma region LL_Info

		__NODISCARD uint64_t Count() const
		{
			uint64_t count{};
			Node* pCurrent{ m_pHead };

			while (pCurrent)
			{
				++count;

				pCurrent = pCurrent->pNext;
			}

			return count;
		}

#pragma endregion

#pragma region LL_Manipulation

		void Clear()
		{
			Node* pNext{};
			Node* pCurrent{ m_pHead };

			const uint64_t size{ Count() };
			for (uint64_t i{}; i < size; ++i)
			{
				if (!pCurrent)
					break;

				pNext = pCurrent->pNext;

				free(pCurrent);
				pCurrent = nullptr;

				pCurrent = pNext;
			}

			m_pHead = nullptr;
		}

#pragma endregion

#pragma region Element_Access

		__NODISCARD T& Get(const uint64_t index)
		{
			__ASSERT(index < Count());

			Node* pCurrent{ m_pHead };

			for (uint64_t i{}; i < index; ++i)
				pCurrent = pCurrent->pNext;

			return pCurrent->Data;
		}

		__NODISCARD const T& Get(const uint64_t index) const
		{
			__ASSERT(index < Count());

			Node* pCurrent{ m_pHead };

			for (uint64_t i{}; i < index; ++i)
				pCurrent = pCurrent->pNext;

			return pCurrent->Data;
		}

		__NODISCARD T& Back()
		{
			return Get(Count() - 1);
		}

		__NODISCARD const T& Back() const
		{
			return Get(Count() - 1);
		}

#pragma endregion

	private:
#pragma region Helpers

		Node* GetLastElement()
		{
			Node* pCurrent{ m_pHead };

			const uint64_t size{ Count() };
			for (uint64_t i{}; i < size; ++i)
				pCurrent = pCurrent->pNext;

			return pCurrent;
		}

#pragma endregion

		Node* m_pHead;
	};
}