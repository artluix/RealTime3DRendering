#pragma once
#include <type_traits>

/////////////////////////////////////////////////////////////////

namespace library
{
    namespace rtti
    {
        namespace detail
        {
            template <class From, class To, bool = std::is_base_of<From, To>::value || std::is_base_of<From, To>::value>
            struct Cast;

            template <class From, class To>
            struct Cast<From, To, false>
            {
                To* operator()(From* const) const
                {
                    return nullptr;
                }
            };

            template <class From, class To>
            struct Cast<From, To, true>
            {
                To* operator()(From* const self) const
                {
                    return static_cast<To*>(self);
                }
            };
        }

        template <class Cls>
        class Class
        {
        public:
            template <class T>
            T* As()
            {
                auto self = static_cast<Cls*>(this);
                return detail::Cast<Cls, T>()(self);
            }

            template <class T>
            const T* As() const
            {
                auto self = const_cast<Cls*>(static_cast<const Cls*>(this));
                return detail::Cast<Cls, T>()(self);
            }

        protected:
            explicit Class() = default;
        };
    }
}
