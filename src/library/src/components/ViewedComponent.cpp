#include "library/components/ViewedComponent.h"

#include "library/components/CameraComponent.h"

namespace library
{
    namespace components
    {
        ViewedComponent::ViewedComponent(const Application& app, const CameraComponent& camera)
            : Class(app)
            , m_camera(camera)
        {
        }

        void ViewedComponent::SetCamera(const CameraComponent& camera)
        {
            m_camera = camera;
        } 
    } // namespace components
} // namespace library
