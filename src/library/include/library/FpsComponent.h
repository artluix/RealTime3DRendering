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
        SpriteBatch* m_spriteBatch;
        SpriteFont* m_spriteFont;

        XMFLOAT2 m_textPosition;
        unsigned m_frameCount;
        unsigned m_frameRate;
        double m_lastTotalElapsedTime; // will be changed!
    };
}
