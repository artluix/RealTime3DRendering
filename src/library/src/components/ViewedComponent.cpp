#include "library/components/ViewedComponent.h"

#include "library/components/CameraComponent.h"

namespace library
{
    namespace components
    {
        Viewed::Viewed(const Application& app, const Camera& camera)
            : Class(app)
            , m_camera(camera)
        {
        }
    }

    /*void ViewedComponent::SetCamera(Camera& camera)
    {
        m_camera = camera;
    }*/
}
