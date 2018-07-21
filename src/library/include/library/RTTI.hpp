#pragma once
#include <cstdint>
#include <type_traits>
#include <tuple>

/////////////////////////////////////////////////////////////////

namespace library
{
    namespace rtti
    {
        using TypeId = std::uintptr_t;

        /////////////////////////////////////////////////////////////////

        class Interface;

        namespace detail
        {
            template <class Type>
            TypeId GetTypeId()
            {
                static const int generator = 0;
                static const auto typeId = reinterpret_cast<TypeId>(&generator);
                return typeId;
            }

            template <class Parent>
            class ParentUnpacker : public Parent
            {
            protected:
                template <class Tuple>
                explicit ParentUnpacker(Tuple&& tuple) : Parent(std::get<Parent&&>(std::forward<Tuple>(tuple))) {}
            };

            /////////////////////////////////////////////////////////////////

            template <class Cls, class Parent, bool = std::is_base_of<Interface, Parent>::value>
            struct ParentCastImpl {};

            template <class Cls, class Parent>
            struct ParentCastImpl<Cls, Parent, true>
            {
                void* operator()(Cls* const self, const TypeId typeId) const
                {
                    return self->Parent::CastTo(typeId);
                }
            };

            template <class Cls, class Parent>
            struct ParentCastImpl<Cls, Parent, false>
            {
                void* operator()(Cls* const self, const TypeId typeId) const
                {
                    if (GetTypeId<Parent>() == typeId)
                        return static_cast<Parent*>(self);

                    return nullptr;
                }
            };

            /////////////////////////////////////////////////////////////////

            template <class, class...>
            struct ParentCast;

            template <class Cls>
            struct ParentCast<Cls>
            {
                void* operator()(Cls* const, const TypeId) const
                {
                    return nullptr;
                }
            };

            template <class Cls, class FirstParent, class... RestParents>
            struct ParentCast<Cls, FirstParent, RestParents...>
            {
                void* operator()(Cls* const self, const TypeId typeId) const
                {
                    if (auto instance = ParentCastImpl<Cls, FirstParent>()(self, typeId))
                        return instance;

                    return ParentCast<Cls, RestParents...>()(self, typeId);
                }
            };

            /////////////////////////////////////////////////////////////////

            template <class, class...>
            struct Cast;

            template <class Cls>
            struct Cast<Cls>
            {
                void* operator()(Cls* const self, const TypeId typeId) const
                {
                    if (GetTypeId<Cls>() == typeId)
                        return self;

                    return nullptr;
                }
            };

            template <class Cls, class FirstParent, class ...RestParents>
            struct Cast<Cls, FirstParent, RestParents...>
            {
                void* operator()(Cls* const self, const TypeId typeId) const
                {
                    if (GetTypeId<Cls>() == typeId)
                        return self;

                    return ParentCast<Cls, FirstParent, RestParents...>()(self, typeId);
                }
            };
        }

        /////////////////////////////////////////////////////////////////

        class Interface
        {
        public:
            virtual ~Interface() = default;

            virtual bool Is(const TypeId typeId) const = 0;
            virtual void* CastTo(const TypeId typeId) = 0;
            virtual const void* CastTo(const TypeId typeId) const = 0;

        protected:
            Interface() = default;
        };

        /////////////////////////////////////////////////////////////////

        template <class Cls, class ...Parents>
        class Class
            : public virtual Interface
            , public detail::ParentUnpacker<Parents>...
        {
        public:
            static TypeId GetTypeId()
            {
                return detail::GetTypeId<Cls>();
            }

            bool Is(const TypeId typeId) const override
            {
                return !!CastTo(typeId);
            }

            void* CastTo(const TypeId typeId) override
            {
                auto self = static_cast<Cls*>(this);
                return detail::Cast<Cls, Parents...>()(self, typeId);
            }

            const void* CastTo(const TypeId typeId) const override
            {
                auto self = const_cast<Cls*>(static_cast<const Cls*>(this));
                return detail::Cast<Cls, Parents...>()(self, typeId);
            }

            template <class T>
            T* As()
            {
                return static_cast<T*>(CastTo(detail::GetTypeId<T>()));
            }

            template <class T>
            const T* As() const
            {
                return static_cast<const T*>(CastTo(detail::GetTypeId<T>()));
            }

        protected:
            template <class ...SomeParents>
            explicit Class(SomeParents&&... someParents)
                : ParentUnpacker<Parents>(std::tuple<SomeParents&&...>(std::forward<SomeParents>(someParents)...))...
            {}
        };
    }
}
