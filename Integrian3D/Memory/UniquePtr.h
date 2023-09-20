#pragma once

#include "EngineConstants.h"

#include <type_traits>

namespace Integrian3D
{
	namespace Detail
	{
		template<typename T>
		struct DefaultDeleter final
		{
			void operator()(T*& Data) const
			{
				if (Data)
				{
					delete Data;
					Data = nullptr;
				}
			}
		};

		template<typename T>
		struct DefaultDeleter<T[]> final
		{
			void operator()(T*& Data) const
			{
				if (Data)
				{
					delete[] Data;
					Data = nullptr;
				}
			}
		};
	}

	template<typename T, typename Deleter = Detail::DefaultDeleter<T>>
	class UniquePtr
	{
		static_assert(std::is_invocable_v<Deleter, T*&>, "UniquePtr<T, Deleter> Deleter must be an invocable type");

	public:
		UniquePtr()
			: Data{}
			, InvocableDeleter{}
		{}

		explicit UniquePtr(T* const&& Pointer) // only allow r-value (const) pointers
			: Data{ Pointer }
			, InvocableDeleter{}
		{}

		~UniquePtr()
		{
			InvocableDeleter(Data);
		}

		UniquePtr(const UniquePtr&) noexcept = delete;
		UniquePtr& operator=(const UniquePtr&) noexcept = delete;
		UniquePtr(UniquePtr&& Other) noexcept
			: Data{ __MOVE(Other.Data) }
			, InvocableDeleter{ __MOVE(Other.InvocableDeleter) }
		{
			Other.Data = nullptr;
		}
		UniquePtr& operator=(UniquePtr&& Other) noexcept
		{
			InvocableDeleter(Data);

			Data = __MOVE(Other.Data);
			InvocableDeleter = __MOVE(Other.InvocableDeleter);

			Other.Data = nullptr;

			return *this;
		}

		void Reset(T* Pointer)
		{
			T* OldPointer{ Data };
			Data = Pointer;

			if (OldPointer)
			{
				delete OldPointer;
				OldPointer = nullptr;
			}
		}

		T* Get()
		{
			return Data;
		}
		const T* Get() const
		{
			return Data;
		}

		T* operator->()
		{
			return Data;
		}
		const T* operator->() const
		{
			return Data;
		}

		T& operator*()
		{
			__CHECK(Data);

			return *Data;
		}
		const T& operator*() const
		{
			__CHECK(Data);

			return *Data;
		}

		explicit operator bool() const
		{
			return Data != nullptr;
		}

		bool operator==(std::nullptr_t) const
		{
			return Data == nullptr;
		}
		bool operator!=(std::nullptr_t) const
		{
			return Data != nullptr;
		}

	private:
		T* Data;
		Deleter InvocableDeleter;
	};

	template<typename T, typename Deleter>
	class UniquePtr<T[], Deleter>
	{
		static_assert(std::is_invocable_v<Deleter, T*&>, "UniquePtr<T, Deleter> Deleter must be an invocable type");

	public:
		UniquePtr()
			: Data{}
			, InvocableDeleter{}
		{}

		explicit UniquePtr(T* const&& Pointer) // only allow r-value (const) pointers
			: Data{ Pointer }
			, InvocableDeleter{}
		{}

		~UniquePtr()
		{
			InvocableDeleter(Data);
		}

		UniquePtr(const UniquePtr&) noexcept = delete;
		UniquePtr& operator=(const UniquePtr&) noexcept = delete;
		UniquePtr(UniquePtr&& Other) noexcept
			: Data{ __MOVE(Other.Data) }
			, InvocableDeleter{ __MOVE(Other.InvocableDeleter) }
		{
			Other.Data = nullptr;
		}
		UniquePtr& operator=(UniquePtr&& Other) noexcept
		{
			InvocableDeleter(Data);

			Data = __MOVE(Other.Data);
			InvocableDeleter = __MOVE(Other.InvocableDeleter);

			Other.Data = nullptr;

			return *this;
		}

		void Reset(T* Pointer)
		{
			T* OldPointer{ Data };
			Data = Pointer;

			if (OldPointer)
			{
				delete OldPointer;
				OldPointer = nullptr;
			}
		}

		T* Get()
		{
			return Data;
		}
		const T* Get() const
		{
			return Data;
		}

		T* operator->()
		{
			return Data;
		}
		const T* operator->() const
		{
			return Data;
		}

		T& operator*()
		{
			__CHECK(Data);

			return *Data;
		}
		const T& operator*() const
		{
			__CHECK(Data);

			return *Data;
		}

		explicit operator bool() const
		{
			return Data != nullptr;
		}

		bool operator==(std::nullptr_t) const
		{
			return Data == nullptr;
		}
		bool operator!=(std::nullptr_t) const
		{
			return Data != nullptr;
		}

	private:
		T* Data;
		Deleter InvocableDeleter;
	};

	template<typename T, typename Deleter = Integrian3D::Detail::DefaultDeleter<T>, typename ... Args, std::enable_if_t<!std::is_array_v<T>, bool> = true>
	__NODISCARD inline UniquePtr<T, Deleter> MakeUnique(Args&& ... Arguments)
	{
		return UniquePtr<T>(new T{ __FORWARD(Arguments)...});
	}

	template<typename T, typename Deleter = Integrian3D::Detail::DefaultDeleter<T>, std::enable_if_t<std::is_array_v<T> && std::extent_v<T> == 0, bool> = true>
	__NODISCARD inline UniquePtr<T, Deleter> MakeUnique(const int32 NrOfElements)
	{
		return UniquePtr<T>(new std::remove_extent_t<T>[NrOfElements]{});
	}

	template<typename T, typename Deleter = Integrian3D::Detail::DefaultDeleter<T>, typename ... Args, std::enable_if_t<std::extent_v<T> != 0, bool> = true>
	void MakeUnique(Args&& ... Arguments) = delete;
}