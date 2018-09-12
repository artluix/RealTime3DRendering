#include "library/components/BaseComponent.h"


namespace library
{
    namespace components
    {
        Base::Base(const Application& app)
            : m_app(app)
            , m_enabled(true)
        {
        }

        void Base::SetEnabled(const bool enabled)
        {
            m_enabled = enabled;
        }

        void Base::Initialize()
        {
        }

        void Base::Update(const Time& time)
        {
        }
    }
}
