#pragma once
#include <type_traits>
#include <cstdint>

/////////////////////////////////////////////////////////////////

namespace library
{
    namespace rtti
    {
        using TypeId = std::uintptr_t;

        template <class T>
        TypeId GetTypeId()
        {
            static const int generator = 0;
            const auto typeId = reinterpret_cast<TypeId>(&generator);
            return typeId;
        }

        /////////////////////////////////////////////////////////////////

        class Base
        {
        public:
            virtual bool Is(const TypeId typeId) const = 0;

        protected:
            virtual ~Base() = default;
        };

        /////////////////////////////////////////////////////////////////

        namespace detail
        {
            template <class Cls, class Parent, bool = std::is_base_of<Base, Parent>::value>
            struct ParentIsImpl;

            template <class Cls, class Parent>
            struct ParentIsImpl<Cls, Parent, true>
            {
                bool operator()(const Cls* self, const TypeId typeId) const
                {
                    return self->Parent::Is(typeId);
                }
            };

            template <class Cls, class Parent>
            struct ParentIsImpl<Cls, Parent, false>
            {
                bool operator()(const Cls*, const TypeId typeId) const
                {
                    return GetTypeId<Parent>() == typeId;
                }
            };
        }

        /////////////////////////////////////////////////////////////////

        template<class...>
        class Class;

        template <class Cls>
        class Class<Cls> : public virtual Base
        {
        public:
            bool Is(const TypeId typeId) const override
            {
                return GetTypeId<Cls>() == typeId;
            }

            template <class T>
            T* As()
            {
                if (Is(GetTypeId<T>()))
                {
                    auto self = static_cast<Cls*>(this);
                    return static_cast<T*>(self);
                }

                return nullptr;
            }

            template <class T>
            const T* As() const
            {
                if (Is(GetTypeId<T>()))
                {
                    auto self = static_cast<const Cls*>(this);
                    return static_cast<const T*>(self);
                }

                return nullptr;
            }

        protected:
            explicit Class() = default;
            virtual ~Class() = default;
        };

        template <class Cls, class Parent>
        class Class<Cls, Parent>
            : public virtual Base
            , public Parent
        {
        public:
            bool Is(const TypeId typeId) const override
            {
                if (GetTypeId<Cls>() == typeId)
                    return true;

                auto self = static_cast<const Cls*>(this);
                return detail::ParentIsImpl<Cls, Parent>()(self, typeId);
            }

            // yes, duplicate, but we should reduce hierarchy nodes count
            template <class T>
            T* As()
            {
                if (Is(GetTypeId<T>()))
                {
                    auto self = static_cast<Cls*>(this);
                    return static_cast<T*>(self);
                }

                return nullptr;
            }

            template <class T>
            const T* As() const
            {
                if (Is(GetTypeId<T>()))
                {
                    auto self = static_cast<const Cls*>(this);
                    return static_cast<const T*>(self);
                }

                return nullptr;
            }

        protected:
            explicit Class() = default;
            ~Class() = default;

            using Parent::Parent;
        };

        // some wrappers

        template <class To, class From, typename = std::enable_if<std::is_base_of<Base, From>::value>::type>
        To* CastTo(From* self)
        {
            return self->As<To>();
        }

        template <class To, class From, typename = std::enable_if<std::is_base_of<Base, From>::value>::type>
        const To* CastTo(const From* self)
        {
            return self->As<To>();
        }
    }
}
