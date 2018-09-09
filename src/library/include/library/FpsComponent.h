#pragma once
#include "library/DrawableComponent.h"

#include <DirectXMath.h>

namespace DirectX
{
    class SpriteBatch;
    class SpriteFont;
}

namespace library
{
    class FpsComponent : public rtti::Class<FpsComponent, DrawableComponent>
    {
    public:
        explicit FpsComponent(const Application& app);
        ~FpsComponent() = default;

        DirectX::XMFLOAT2 GetTextPosition() const { return m_textPosition; }
        void SetTextPosition(const DirectX::XMFLOAT2& position);

        unsigned GetFrameRate() const { return m_frameRate; }

        void Initialize() override;
        void Update(const Time& time) override;
        void Draw(const Time& time) override;

    private:
        std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
        std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

        DirectX::XMFLOAT2 m_textPosition;
        unsigned m_frameCount;
        unsigned m_frameRate;

        Duration m_timeAccumulator;
    };
}
