#include "library/ViewedComponent.h"

namespace library
{
    ViewedComponent::ViewedComponent(Application& app, Camera& camera)
        : DrawableComponent(app)
        , m_camera(camera)
    {
    }

    Camera& ViewedComponent::GetCamera()
    {
        return m_camera;
    }

    void ViewedComponent::SetCamera(Camera& camera)
    {
        m_camera = camera;
    }
}


