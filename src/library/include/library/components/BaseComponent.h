#pragma once
#include "library/NonCopyable.hpp"
#include "library/Application.h"
#include "library/RTTI.hpp"

namespace library
{
    namespace components
    {
        class Base
            : public NonCopyable<Base>
            , public rtti::Class<Base>
        {
        public:
            explicit Base(const Application& app);
            virtual ~Base() = default;

            bool IsEnabled() const { return m_enabled; }
            void SetEnabled(const bool enabled);

            virtual void Initialize();
            virtual void Update(const Time& time);

        protected:
            const Application& m_app;
            bool m_enabled;
        };
    }
}
