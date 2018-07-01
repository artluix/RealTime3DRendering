#include "library/DrawableComponent.h"

namespace library
{
    DrawableComponent::DrawableComponent(Application& app, Camera& camera)
        : RTTI(app)
        , m_camera(camera)
    {
    }

    bool DrawableComponent::IsVisible() const
    {
        return m_visible;
    }

    void DrawableComponent::SetVisible(const bool visible)
    {
        m_visible = visible;
    }

    Camera& DrawableComponent::GetCamera()
    {
        return m_camera;
    }

    void DrawableComponent::SetCamera(Camera& camera)
    {
        m_camera = camera;
    }

}