#pragma once

#include "EngineConstants.h"

namespace Integrian3D
{
	template<typename T>
	class UniquePtr
	{
	public:
		UniquePtr();
		explicit UniquePtr(T* const&& Pointer); // only allow r-value (const) pointers
		~UniquePtr();

		UniquePtr(const UniquePtr&) noexcept = delete;
		UniquePtr(UniquePtr&& Other) noexcept;
		UniquePtr& operator=(const UniquePtr&) noexcept = delete;
		UniquePtr& operator=(UniquePtr&& Other) noexcept;

		void Reset(T* const&& Pointer);

		T* Get();
		const T* Get() const;

		T* operator->();
		const T* operator->() const;

		T& operator*();
		const T& operator*() const;

		explicit operator bool() const;
		bool operator==(std::nullptr_t) const;

	private:
		T* Data;
	};

	template<typename T>
	UniquePtr<T>::UniquePtr()
		: Data{}
	{}

	template<typename T>
	UniquePtr<T>::UniquePtr(T* const&& Pointer)
		: Data{ Pointer }
	{}

	template<typename T>
	UniquePtr<T>::~UniquePtr()
	{
		if (Data)
		{
			delete Data;
			Data = nullptr;
		}
	}

	template<typename T>
	UniquePtr<T>::UniquePtr(UniquePtr&& Other) noexcept
		: Data{ __MOVE(Other.Data) }
	{
		Other.Data = nullptr;
	}

	template<typename T>
	UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr&& Other) noexcept
	{
		if (Data)
		{
			delete Data;
			Data = nullptr;
		}

		Data = __MOVE(Other.Data);
		Other.Data = nullptr;

		return *this;
	}

	template<typename T>
	void UniquePtr<T>::Reset(T* const&& Pointer)
	{
		T* OldPointer{ Data };
		Data = Pointer;

		if (OldPointer)
		{
			delete OldPointer;
			OldPointer = nullptr;
		}
	}

	template<typename T>
	T* UniquePtr<T>::Get()
	{
		return Data;
	}

	template<typename T>
	const T* UniquePtr<T>::Get() const
	{
		return Data;
	}

	template<typename T>
	T* UniquePtr<T>::operator->()
	{
		return Data;
	}

	template<typename T>
	const T* UniquePtr<T>::operator->() const
	{
		return Data;
	}

	template<typename T>
	T& UniquePtr<T>::operator*()
	{
		__CHECK(Data);

		return *Data;
	}

	template<typename T>
	const T& UniquePtr<T>::operator*() const
	{
		__CHECK(Data);

		return *Data;
	}

	template<typename T>
	UniquePtr<T>::operator bool() const
	{
		return Data != nullptr;
	}

	template<typename T>
	bool UniquePtr<T>::operator==(std::nullptr_t) const
	{
		return Data == nullptr;
	}

	template<typename T, typename ... Args>
	static UniquePtr<T> MakeUnique(Args&& ... Arguments)
	{
		return UniquePtr<T>(new T{ Arguments... });
	}
}