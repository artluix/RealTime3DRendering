#include "library/components/FpsComponent.h"

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
        const auto k_fontPath = utils::GetExecutableDirectory().Join(
            filesystem::Path(L"data/fonts/Arial_14_Regular.spritefont")
        );
        const auto k_fpsMeasureInterval = std::chrono::seconds(1);
    }

    namespace components
    {
        Fps::Fps(const Application& app)
            : Class(app)
            , m_textPosition(0.0f, 0.0f)
            , m_frameCount(0)
            , m_frameRate(0)
            , m_timeAccumulator(Duration::zero())
        {
        }

        void Fps::SetTextPosition(const DirectX::XMFLOAT2& position)
        {
            m_textPosition = position;
        }

        void Fps::Initialize()
        {
            m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_app.GetD3DDeviceContext());
            m_spriteFont = std::make_unique<DirectX::SpriteFont>(m_app.GetD3DDevice(), k_fontPath.GetAsWideCString());
        }

        void Fps::Update(const Time& time)
        {
            const auto elapsed = time.elapsed.GetDuration();
            if (m_timeAccumulator >= k_fpsMeasureInterval)
            {
                m_frameRate = m_frameCount;

                m_timeAccumulator = Duration::zero();
                m_frameCount = 0;
            }
            else
            {
                m_timeAccumulator += elapsed;
            }

            m_frameCount++;
        }

        void Fps::Draw(const Time& time)
        {
            m_spriteBatch->Begin();

            std::wostringstream woss;
            woss << std::setprecision(4) <<
                L"Frame Rate: " << m_frameRate << std::endl << L"Total Elapsed Time: " << time.total.GetSeconds<float>();

            m_spriteFont->DrawString(m_spriteBatch.get(), woss.str().c_str(), m_textPosition, DirectX::Colors::White);

            m_spriteBatch->End();
        }
    }
}
