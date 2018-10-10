#pragma once
#include "library/components/DrawableComponent.h"
#include "library/components/CameraComponent.h"

namespace library
{
    namespace components
    {
        class ViewedComponent : public rtti::Class<ViewedComponent, DrawableComponent>
        {
        public:
            explicit ViewedComponent(const Application& app, const CameraComponent& camera);
            ~ViewedComponent() = default;

            const CameraComponent& GetCamera() { return m_camera; }
            void SetCamera(const CameraComponent& camera);

        protected:
            std::reference_wrapper<const CameraComponent> m_camera;
        };
    } // namespace components
} // namespace library
