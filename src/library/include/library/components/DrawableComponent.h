#pragma once
#include "library/components/BaseComponent.h"
#include "library/Stopwatch.h"

namespace library
{
    namespace components
    {
        class Drawable : public rtti::Class<Drawable, Base>
        {
        public:
            explicit Drawable(const Application& app);
            ~Drawable() = default;

            bool IsVisible() const { return m_visible; }
            void SetVisible(const bool visible);

            virtual void Draw(const Time& time);

        protected:
            bool m_visible;
        };
    }
}
