#include "library/components/DrawableComponent.h"

namespace library
{
    namespace components
    {
        DrawableComponent::DrawableComponent(const Application& app)
            : Class(app)
            , m_visible(false)
        {
        }

        void DrawableComponent::SetVisible(const bool visible)
        {
            m_visible = visible;
        }

        void DrawableComponent::Draw(const Time& time)
        {
        }
    } // namespace components
} // namespace library
