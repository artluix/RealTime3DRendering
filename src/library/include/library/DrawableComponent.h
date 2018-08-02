#pragma once
#include "library/Component.h"
#include "library/Clock.h"

namespace library
{
    class DrawableComponent : public Component
    {
    public:
        using Component::Component;
        ~DrawableComponent() = default;

        bool IsVisible() const;
        void SetVisible(const bool visible);

        virtual void Draw(const Time& time);

    protected:
        bool m_visible;
    };
}
