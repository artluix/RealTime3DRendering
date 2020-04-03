#pragma once
#include "library/NonCopyable.h"

#include <Unknwn.h>

namespace library
{
template <typename T>
class ComPtr : public NonCopyable<ComPtr<T>>
{
public:
	using Type = T;

	constexpr explicit ComPtr(T* ptr = nullptr) noexcept : m_ptr(ptr) {}

	ComPtr(ComPtr&& other) noexcept : m_ptr(other.m_ptr) { other.m_ptr = nullptr; }

	~ComPtr() noexcept { _Release(); }

	ComPtr& operator=(ComPtr&& other) noexcept
	{
		_Release();
		Swap(other);
		return *this;
	}

	ComPtr& operator=(T* ptr) noexcept
	{
		Reset(ptr);
		return *this;
	}

	//-------------------------------------------------------------------------

	T* Get() const noexcept { return m_ptr; }
	T*& Get() noexcept { return m_ptr; }

	void Reset(T* ptr = nullptr) noexcept
	{
		_Release();
		m_ptr = ptr;
	}

	void Swap(ComPtr& other) noexcept { std::swap(m_ptr, other.m_ptr); }

	//-------------------------------------------------------------------------

	template <typename U>
	HRESULT As(ComPtr<U>& other) const
	{
		return m_ptr->QueryInterface(__uuidof(U), reinterpret_cast<void**>(&other));
	}

	//-------------------------------------------------------------------------

	T* const* operator&() const noexcept { return &m_ptr; }
	T** operator&() noexcept { return &m_ptr; }

	T* operator->() const noexcept { return m_ptr; }
	T& operator*() const noexcept { return *m_ptr; }

	explicit operator bool() const noexcept { return !!m_ptr; }

private:
	void _Release() noexcept
	{
		if (!!m_ptr)
		{
			m_ptr->Release();
			m_ptr = nullptr;
		}
	}

	T* m_ptr;
};
} // namespace library
