#include "StdAfx.h"
#include "library/Exception.h"

namespace library
{
	Exception::Exception(const char* const& message, const HRESULT hr /*= S_OK*/)
		: exception(message)
		, m_hr(hr)
	{
	}

	std::string Exception::GetWhatString() const
	{
		std::string msg = what();

		if (m_hr != S_OK)
			msg += " Error: " + std::to_string(m_hr);

		return msg;
	}
} // namespace library
