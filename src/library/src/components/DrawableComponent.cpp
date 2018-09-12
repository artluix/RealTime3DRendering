#include "library/components/DrawableComponent.h"

namespace library
{
    namespace components
    {
        Drawable::Drawable(const Application& app)
            : Class(app)
            , m_visible(false)
        {
        }

        void Drawable::SetVisible(const bool visible)
        {
            m_visible = visible;
        }

        void Drawable::Draw(const Time& time)
        {
        }
    }
}
