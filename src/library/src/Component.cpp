#include "library/Component.h"


namespace library
{

    Component::Component(Application& app)
        : m_app(app)
        , m_enabled(true)
    {
    }

    Component::~Component()
    {
    }

    Application& Component::GetApplication()
    {
        return m_app;
    }

    void Component::SetApplication(Application& app)
    {
        m_app = app;
    }

    bool Component::IsEnabled() const
    {
        return m_enabled;
    }

    void Component::SetEnabled(const bool enabled)
    {
        m_enabled = true;
    }

    void Component::Initialize()
    {

    }

    void Component::Update(const Time& time)
    {
    }
}
