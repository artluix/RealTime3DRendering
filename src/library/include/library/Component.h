#pragma once
#include "library/NonCopyable.hpp"
#include "library/Application.h"
#include "library/RTTI.hpp"

//#include <functional>

namespace library
{
    class Component
        : public NonCopyable<Component>
        , public rtti::Class<Component>
    {
    public:
        explicit Component(const Application& app);
        virtual ~Component() = default;

        //Application& GetApplication();
        //void SetApplication(const Application& app);

        bool IsEnabled() const { return m_enabled; }
        void SetEnabled(const bool enabled);

        virtual void Initialize();
        virtual void Update(const Time& time);

    protected:
        //std::reference_wrapper<const Application> m_app;
        const Application& m_app;
        bool m_enabled;
    };
}
