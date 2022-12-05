#pragma once

#include "../EngineConstants.h"
#include "../Iterator/Iterator.h"

namespace Integrian3D
{
	/* [TODO]: Conform to STL */
	template<typename T>
	class LinkedList final
	{
	private:
		using It = Iterator<T, IteratorTag::ForwardIt>;
		using CIt = ConstIterator<T, IteratorTag::ForwardIt>;

	public:
		struct Node final
		{
			constexpr Node(const T& data)
				: Data{ data }
				, pNext{}
			{}

			T Data;
			Node* pNext;
		};

		LinkedList() = default;
		~LinkedList()
		{
			Clear();
		}

#pragma region RuleOf5
		LinkedList(const LinkedList& other) noexcept
			: m_pHead{}
			, m_Size{}
		{
			for (uint64_t i{}; i < other.Size(); ++i)
				Add(*other.At(i));
		}
		LinkedList(LinkedList&& other) noexcept
			: m_pHead{ __MOVE(Node*, other.m_pHead) }
			, m_Size{ __MOVE(uint64_t, other.m_Size) }
		{
			other.m_pHead = nullptr;
		}
		LinkedList& operator=(const LinkedList& other) noexcept
		{
			if (m_pHead)
				Clear();

			for (uint64_t i{}; i < other.Size(); ++i)
				Add(*other.At(i));

			return *this;
		}
		LinkedList& operator=(LinkedList&& other) noexcept
		{
			if (m_pHead)
				Clear();

			m_pHead = __MOVE(Node*, m_pHead);
			m_Size = __MOVE(uint64_t, other.m_Size);

			other.m_pHead = nullptr;

			return *this;
		}
#pragma endregion

#pragma region AddandRemove
		constexpr void Add(const T& value)
		{
			EmplaceBack(value);
		}
		constexpr void Add(T&& value)
		{
			EmplaceBack(__MOVE(T, value));
		}

		constexpr void AddFront(const T& value)
		{
			EmplaceFront(value);
		}
		constexpr void AddFront(T&& value)
		{
			EmplaceFront(__MOVE(T, value));
		}

		constexpr void Erase(const T& value)
		{
			Node* pPrevious{};
			Node* pCurrent{ m_pHead };
			for (uint64_t i{}; i < m_Size; ++i)
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
					delete pCurrent;
					pCurrent = nullptr;

					pCurrent = pNext;

					--m_Size;

					continue;
				}

				pCurrent = pCurrent->pNext;
				pPrevious = pCurrent;
			}
		}

		template<typename ... Ts>
		constexpr T& EmplaceBack(Ts&&... args)
		{
			++m_Size;

			/* first node to be added */
			if (!m_pHead)
			{
				m_pHead = new Node{ T{ __FORWARD(Ts, args)...} };
				return m_pHead->Data;
			}
			else
			{
				Node* pLastElement{ GetLastElement() };
				Node* pNewElement{ new Node{ T{ __FORWARD(Ts, args)...} } };

				pLastElement->pNext = pNewElement;

				return pNewElement->Data;
			}
		}

		template<typename ... Ts>
		constexpr T& EmplaceFront(Ts&&... args)
		{
			++m_Size;

			/* first node to be added */
			if (!m_pHead)
			{
				m_pHead = new Node{ T{ __FORWARD(Ts, args)...} };
				return m_pHead->Data;
			}
			else
			{
				Node* pNewElement{ new Node{ T{ __FORWARD(Ts, args)...} } };

				pNewElement->pNext = m_pHead;
				m_pHead = pNewElement;

				return pNewElement->Data;
			}
		}
#pragma endregion

#pragma region Manipulation
		constexpr void Clear()
		{
			Node* pNext{};
			Node* pCurrent{ m_pHead };

			for (uint64_t i{}; i < m_Size; ++i)
			{
				if (!pCurrent)
					break;

				pNext = pCurrent->pNext;

				delete pCurrent;
				pCurrent = nullptr;

				pCurrent = pNext;
			}

			m_pHead = nullptr;
			m_Size = 0;
		}
#pragma endregion

#pragma region Accessing
		__NODISCARD constexpr It Front()
		{
			__ASSERT(m_pHead);
			return It{ &m_pHead->Data };
		}
		__NODISCARD constexpr CIt Front() const
		{
			__ASSERT(m_pHead);
			return CIt{ &m_pHead->Data };
		}

		__NODISCARD constexpr It Back()
		{
			Node* pCurrent{ m_pHead };

			while (pCurrent->pNext)
			{
				pCurrent = pCurrent->pNext;
			}

			__ASSERT(pCurrent);

			return It{ &pCurrent->Data };
		}
		__NODISCARD constexpr CIt Back() const
		{
			Node* pCurrent{ m_pHead };

			while (pCurrent->pNext)
			{
				pCurrent = pCurrent->pNext;
			}

			__ASSERT(pCurrent);

			return CIt{ &pCurrent->Data };
		}

		__NODISCARD constexpr It At(const uint64_t n)
		{
			__ASSERT(n < m_Size);

			Node* pCurrent{ m_pHead };
			for (uint64_t i{}; i < n; ++i)
			{
				pCurrent = pCurrent->pNext;
			}

			return It{ &pCurrent->Data };
		}
		__NODISCARD constexpr CIt At(const uint64_t n) const
		{
			__ASSERT(n < m_Size);

			Node* pCurrent{ m_pHead };
			for (uint64_t i{}; i < n; ++i)
			{
				pCurrent = pCurrent->pNext;
			}

			return CIt{ &pCurrent->Data };
		}

		__NODISCARD constexpr It Find(const T& value)
		{
			Node* pPrevious{};
			Node* pCurrent{ m_pHead };
			for (uint64_t i{}; i < m_Size; ++i)
			{
				if (!pCurrent)
					return;

				if (pCurrent->Data == value)
				{
					return It{ &pCurrent->Data };
				}

				pCurrent = pCurrent->pNext;
				pPrevious = pCurrent;
			}
		}
		__NODISCARD constexpr CIt Find(const T& value) const
		{
			Node* pPrevious{};
			Node* pCurrent{ m_pHead };
			for (uint64_t i{}; i < m_Size; ++i)
			{
				if (!pCurrent)
					return;

				if (pCurrent->Data == value)
				{
					return CIt{ &pCurrent->Data };
				}

				pCurrent = pCurrent->pNext;
				pPrevious = pCurrent;
			}
		}
#pragma endregion

#pragma region Info
		__NODISCARD constexpr uint64_t Size() const
		{
			return m_Size;
		}
#pragma endregion

#pragma region Iterators
		/* [TODO]: FIX */
		constexpr It begin() { return &m_pHead->Data; }
		constexpr CIt begin() const { return &m_pHead->Data; }

		constexpr It end() { return &(m_pHead + m_Size)->Data - 1; }
		constexpr CIt end() const { return &(m_pHead + m_Size)->Data - 1; }

		constexpr CIt cbegin() const { return m_pHead; }
		constexpr CIt cend() const { return &(m_pHead + m_Size)->Data - 1; }
#pragma endregion

	private:
		Node* GetLastElement() const
		{
			Node* pCurrent{ m_pHead };

			while (pCurrent->pNext)
			{
				pCurrent = pCurrent->pNext;
			}

			return pCurrent;
		}

		Node* m_pHead;
		uint64_t m_Size;
	};
}