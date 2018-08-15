#pragma once
#include "library/DrawableComponent.h"

namespace DirectX
{
    class SpriteBatch;
    class SpriteFont;
}

namespace library
{
    class FpsComponent : public DrawableComponent
    {
    public:
        explicit FpsComponent(Application& app);
        ~FpsComponent() = default;

        XMFLOAT2 GetTextPosition() const;
        void SetTextPosition(const XMFLOAT2& position);

        unsigned GetFrameRate() const;

        void Initialize() override;
        void Update(const Time& time) override;
        void Draw(const Time& time) override;

    private:
        std::unique_ptr<SpriteBatch> m_spriteBatch;
        std::unique_ptr<SpriteFont> m_spriteFont;

        XMFLOAT2 m_textPosition;
        unsigned m_frameCount;
        unsigned m_frameRate;
        unsigned m_totalTimeAccumulator; // reset frame count after 1 second (1000ms)
    };
}
