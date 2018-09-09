#pragma once
#include "library/Component.h"
#include "library/Stopwatch.h"

namespace library
{
    class DrawableComponent : public rtti::Class<DrawableComponent, Component>
    {
    public:
        explicit DrawableComponent(const Application& app);
        ~DrawableComponent() = default;

        bool IsVisible() const { return m_visible; }
        void SetVisible(const bool visible);

        virtual void Draw(const Time& time);

    protected:
        bool m_visible;
    };
}
