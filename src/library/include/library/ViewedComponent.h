#pragma once
#include "library/DrawableComponent.h"
#include "library/Camera.h"

namespace library
{
    class ViewedComponent : public DrawableComponent
    {
    public:
        explicit ViewedComponent(Application& app, Camera& camera);
        ~ViewedComponent() = default;

        Camera& GetCamera();
        void SetCamera(Camera& camera);

    protected:
        std::reference_wrapper<Camera> m_camera;
    };
}
