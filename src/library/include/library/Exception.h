#pragma once
#include <Windows.h>
#include <exception>
#include <string>

namespace library
{

	class Exception : public std::exception
	{
	public:
		explicit Exception(const char* const& message, const HRESULT hr = S_OK);

		HRESULT GetResultHandle() const { return m_hr; }
		std::string GetWhatString() const;

	private:
		HRESULT m_hr;
	};

} // namespace library
