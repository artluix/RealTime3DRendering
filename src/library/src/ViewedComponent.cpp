#include "library/ViewedComponent.h"

namespace library
{
    ViewedComponent::ViewedComponent(const Application& app, const Camera& camera)
        : Class(app)
        , m_camera(camera)
    {
    }

    /*void ViewedComponent::SetCamera(Camera& camera)
    {
        m_camera = camera;
    }*/
}
