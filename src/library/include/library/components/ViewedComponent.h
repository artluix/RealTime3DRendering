#pragma once
#include "library/components/DrawableComponent.h"

namespace library
{
    namespace components
    {
        class Camera;

        class Viewed : public rtti::Class<Viewed, Drawable>
        {
        public:
            explicit Viewed(const Application& app, const Camera& camera);
            ~Viewed() = default;

            const Camera& GetCamera() { return m_camera; }
            //void SetCamera(Camera& camera);

        protected:
            //std::reference_wrapper<Camera> m_camera;
            const Camera& m_camera;
        };
    }
}
