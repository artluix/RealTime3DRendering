#pragma once
#include "library/NonCopyable.hpp"
#include "library/Application.h"
#include "library/RTTI.hpp"

namespace library
{
    namespace components
    {
        class BaseComponent
            : public NonCopyable<BaseComponent>
            , public rtti::Class<BaseComponent>
        {
        public:
            explicit BaseComponent(const Application& app);
            virtual ~BaseComponent() = default;

            bool IsEnabled() const { return m_enabled; }
            void SetEnabled(const bool enabled);

            virtual void Initialize();
            virtual void Update(const Time& time);

        protected:
            const Application& m_app;
            bool m_enabled;
        };
    } // namespace components
} // namespace library
