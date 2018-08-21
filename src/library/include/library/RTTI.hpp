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

        template <class To, class From>
        To* CastTo(From* f)
        {
            return detail::Cast<From, To>()(f);
        }

        template <class To, class From>
        const To* CastTo(const From* f)
        {
            return detail::Cast<From, To>()(f);
        }
    }
}
