#pragma once
#include <type_traits>
#include <cstdint>
#include <memory>

namespace library
{
	namespace rtti
	{
		using TypeId = std::uintptr_t;

		class Base
		{
		public:
			virtual bool Is(const TypeId typeId) const = 0;

		protected:
			virtual ~Base() = default;
		};

		//-------------------------------------------------------------------------

		namespace detail
		{
			template <class T>
			TypeId GetTypeIdImpl()
			{
				static constexpr int generator = 0;
				const auto typeId = reinterpret_cast<TypeId>(&generator);
				return typeId;
			}

			template <class Cls, class Parent, bool = std::is_base_of_v<Base, Parent>>
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
					return GetTypeIdImpl<Parent>() == typeId;
				}
			};
		}

		//-------------------------------------------------------------------------

		template<class...>
		class Class;

		template <class Cls>
		class Class<Cls> : public virtual Base
		{
		public:
			bool Is(const TypeId typeId) const override
			{
				return GetTypeId() == typeId;
			}

			TypeId GetTypeId() const { return detail::GetTypeIdImpl<Cls>(); }

			template <class T>
			T* As()
			{
				if (Is(detail::GetTypeIdImpl<T>()))
				{
					auto self = static_cast<Cls*>(this);
					return static_cast<T*>(self);
				}

				return nullptr;
			}

			template <class T>
			const T* As() const
			{
				if (Is(detail::GetTypeIdImpl<T>()))
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
				if (GetTypeId() == typeId)
					return true;

				auto self = static_cast<const Cls*>(this);
				return detail::ParentIsImpl<Cls, Parent>()(self, typeId);
			}

			TypeId GetTypeId() const { return detail::GetTypeIdImpl<Cls>(); }

			// it's duplicated to reduce hierarchy nodes count
			template <class T>
			T* As()
			{
				if (Is(detail::GetTypeIdImpl<T>()))
				{
					auto self = static_cast<Cls*>(this);
					return static_cast<T*>(self);
				}

				return nullptr;
			}

			template <class T>
			const T* As() const
			{
				if (Is(detail::GetTypeIdImpl<T>()))
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

		// wrappers

		template <class To, class From, typename = std::enable_if_t<std::is_base_of_v<Base, From>>>
		To* CastTo(From* self)
		{
			if (!!self)
				return self->As<To>();

			return nullptr;
		}

		template <class To, class From, typename = std::enable_if_t<std::is_base_of_v<Base, From>>>
		const To* CastTo(const From* self)
		{
			if (!!self)
				return self->As<To>();

			return nullptr;
		}

		template <class To, class From, typename = std::enable_if_t<std::is_base_of_v<Base, From>>>
		std::shared_ptr<To> CastTo(std::shared_ptr<From> self)
		{
			if (!!self && self->Is(detail::GetTypeIdImpl<To>()))
			{
				return std::static_pointer_cast<To>(self);
			}

			return std::shared_ptr<To>();
		}
	} // namespace rtti
} // namespace library
