#pragma once
#include "library/DrawableComponent.h"
#include "library/Camera.h"

namespace library
{
    class ViewedComponent : public rtti::Class<ViewedComponent, DrawableComponent>
    {
    public:
        explicit ViewedComponent(const Application& app, const Camera& camera);
        ~ViewedComponent() = default;

        const Camera& GetCamera() { return m_camera; }
        //void SetCamera(Camera& camera);

    protected:
        //std::reference_wrapper<Camera> m_camera;
        const Camera& m_camera;
    };
}
