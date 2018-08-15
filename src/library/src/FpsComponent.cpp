#include "library/FpsComponent.h"

#include "library/Path.h"
#include "library/Utils.h"

#include <SpriteFont.h>
#include <SpriteBatch.h>
#include <DirectXColors.h>

#include <sstream>
#include <iomanip>

namespace library
{
    namespace
    {
        const auto FontPath =
            utils::GetExecutableDirectory().Join(filesystem::Path(L"data/fonts/Arial_14_Regular.spritefont"));
    }

    FpsComponent::FpsComponent(Application& app)
        : DrawableComponent(app)
        , m_textPosition(0.0f, 0.0f)
        , m_frameCount(0)
        , m_frameRate(0)
        , m_totalTimeAccumulator(0)
    {
    }

    XMFLOAT2 FpsComponent::GetTextPosition() const
    {
        return m_textPosition;
    }

    void FpsComponent::SetTextPosition(const XMFLOAT2& position)
    {
        m_textPosition = position;
    }

    unsigned FpsComponent::GetFrameRate() const
    {
        return m_frameRate;
    }

    void FpsComponent::Initialize()
    {
        Application& app = m_app;

        m_spriteBatch = std::make_unique<SpriteBatch>(app.GetD3DDeviceContext());
        m_spriteFont = std::make_unique<SpriteFont>(app.GetD3DDevice(), FontPath.GetAsWideCString());
    }

    void FpsComponent::Update(const Time& time)
    {
        const auto totalSeconds = time.total.GetSeconds<unsigned>();
        if (totalSeconds - m_totalTimeAccumulator >= 1)
        {
            m_totalTimeAccumulator = totalSeconds;
            m_frameRate = m_frameCount;
            m_frameCount = 0;
        }

        m_frameCount++;
    }

    void FpsComponent::Draw(const Time& time)
    {
        m_spriteBatch->Begin();

        std::wstringstream wss;
        wss << std::setprecision(4) <<
            L"Frame Rate: " << m_frameRate << std::endl << L"Total Elapsed Time: " << time.total.GetSeconds<float>();

        m_spriteFont->DrawString(m_spriteBatch.get(), wss.str().c_str(), m_textPosition, DirectX::Colors::White);

        m_spriteBatch->End();
    }
}
