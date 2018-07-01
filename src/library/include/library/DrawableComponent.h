#pragma once
#include "library/Component.h"
#include "library/Camera.h"

namespace library
{
    class DrawableComponent
        : public RTTI<DrawableComponent, Component>
    {
    public:
        explicit DrawableComponent(Application& app, Camera& camera);

        ~DrawableComponent() = default;

        bool IsVisible() const;
        void SetVisible(const bool visible);

        Camera& GetCamera();
        void SetCamera(Camera& camera);

    protected:
        std::reference_wrapper<Camera> m_camera;
        bool m_visible;
    };
}
