#include "library/ViewedComponent.h"

namespace library
{
    ViewedComponent::ViewedComponent(const Application& app, Camera& camera)
        : Class(app)
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
