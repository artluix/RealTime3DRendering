#include "library/DrawableComponent.h"

namespace library
{
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
