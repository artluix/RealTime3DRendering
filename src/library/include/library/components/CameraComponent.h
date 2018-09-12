#pragma once
#include "library/components/BaseComponent.h"
#include "library/RTTI.hpp"

namespace library
{
    namespace components
    {
        class Camera : public rtti::Class<Camera, Base>
        {
        };
    }
}

