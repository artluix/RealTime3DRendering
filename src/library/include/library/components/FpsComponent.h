#pragma once
#include "library/components/DrawableComponent.h"
#include "library/Math.h"

namespace DirectX
{
    class SpriteBatch;
    class SpriteFont;
}

namespace library
{
    namespace components
    {
        class FpsComponent : public rtti::Class<FpsComponent, DrawableComponent>
        {
        public:
            explicit FpsComponent(const Application& app);
            ~FpsComponent() = default;

            math::Vector2f GetTextPosition() const { return m_textPosition; }
            void SetTextPosition(const math::Vector2f& position) { m_textPosition = position; }

            unsigned GetFrameRate() const { return m_frameRate; }

            void Initialize() override;
            void Update(const Time& time) override;
            void Draw(const Time& time) override;

        private:
            std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
            std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

            math::Vector2f m_textPosition;
            unsigned m_frameCount;
            unsigned m_frameRate;

            Duration m_timeAccumulator;
        };
    } // namespace components
} // namespace library
