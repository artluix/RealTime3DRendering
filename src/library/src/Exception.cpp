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
        return std::string(what());
    }
}
