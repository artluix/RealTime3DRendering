#include "library/Exception.h"


namespace library
{
    Exception::Exception(const char* const& message, const HRESULT hr /*= S_OK*/)
        : exception(message)
        , m_hr(hr)
    {
    }

    std::wstring Exception::GetWhatWString() const
    {
        std::string whatString(what());
        std::wstring whatWString(whatString.begin(), whatString.end());
        return whatWString;
    }
}
