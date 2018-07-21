#pragma once
#include "library/Common.h"
#include "library/NonCopyable.hpp"
#include "library/Application.h"
#include "library/RTTI.hpp"

namespace library
{
    class Component 
        : public NonCopyable<Component>
        , public rtti::Class<Component>
    {
    public:
        explicit Component(Application& app);

        Application& GetApplication();
        void SetApplication(Application& app);

        bool IsEnabled() const;
        void SetEnabled(const bool enabled);

        virtual void Initialize();
        virtual void Update(const Time& time);

    protected:
        std::reference_wrapper<Application> m_app;
        bool m_enabled;
    };
}
