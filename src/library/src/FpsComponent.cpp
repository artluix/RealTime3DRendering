#include "library/FpsComponent.h"

namespace library
{
    FpsComponent::FpsComponent(Application& app)
        : DrawableComponent(app)
    {
    }

    XMFLOAT2 FpsComponent::GetTextPosition() const
    {
        return m_textPosition;
    }

    void FpsComponent::SetTextPosition(const XMFLOAT2& position)
    {

    }

    unsigned FpsComponent::GetFrameRate() const
    {
        return m_frameRate;
    }

    void FpsComponent::Initialize()
    {

    }

    void FpsComponent::Update(const Time& time)
    {

    }

    void FpsComponent::Draw(const Time& time)
    {

    }
}

