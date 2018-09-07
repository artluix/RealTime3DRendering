#pragma once
#include "library/DrawableComponent.h"
#include "library/Camera.h"

namespace library
{
    class ViewedComponent : public rtti::Class<ViewedComponent, DrawableComponent>
    {
    public:
        explicit ViewedComponent(const Application& app, Camera& camera);
        ~ViewedComponent() = default;

        Camera& GetCamera();
        void SetCamera(Camera& camera);

    protected:
        std::reference_wrapper<Camera> m_camera;
    };
}
