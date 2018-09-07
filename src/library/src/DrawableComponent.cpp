#include "library/DrawableComponent.h"

namespace library
{
    DrawableComponent::DrawableComponent(const Application& app)
        : Class(app)
    {
    }

    bool DrawableComponent::IsVisible() const
    {
        return m_visible;
    }

    void DrawableComponent::SetVisible(const bool visible)
    {
        m_visible = visible;
    }

    void DrawableComponent::Draw(const Time& time)
    {
    }
}
