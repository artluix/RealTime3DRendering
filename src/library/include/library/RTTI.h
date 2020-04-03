#pragma once
#include <cstdint>
#include <memory>

namespace library::rtti
{
using TypeId = std::uintptr_t;

template <class T>
TypeId GetTypeId()
{
	static constexpr int generator = 0;
	const auto typeId = reinterpret_cast<TypeId>(&generator);
	return typeId;
}

namespace detail
{
template <class Class, class Parent>
void* ParentCastImpl(Class* const self, const TypeId typeId)
{
	if (GetTypeId<Parent>() == typeId)
		return static_cast<Parent*>(self);

	return self->Parent::CastTo(typeId);
}

//-------------------------------------------------------------------------

template <class, class...>
struct ParentCast;

template <class Class>
struct ParentCast<Class>
{
	void* operator()(Class* const, const TypeId) const { return nullptr; }
};

template <class Class, class FirstParent, class... RestParents>
struct ParentCast<Class, FirstParent, RestParents...>
{
	void* operator()(Class* const self, const TypeId typeId) const
	{
		if (auto instance = ParentCastImpl<Class, FirstParent>(self, typeId))
			return instance;

		return ParentCast<Class, RestParents...>()(self, typeId);
	}
};
} // namespace detail

//-------------------------------------------------------------------------

template <class To, class From>
To* CastTo(From* self)
{
	if (!!self)
		return self->template As<To>();

	return nullptr;
}

template <class To, class From>
const To* CastTo(const From* self)
{
	if (!!self)
		return self->template As<To>();

	return nullptr;
}

template <class To, class From>
std::shared_ptr<To> CastTo(std::shared_ptr<From> self)
{
	if (!!self && self->Is(detail::GetTypeIdImpl<To>()))
	{
		return std::static_pointer_cast<To>(self);
	}

	return std::shared_ptr<To>();
}
} // namespace library::rtti

//-------------------------------------------------------------------------

#define RTTI_CLASS_BASE(Class)														\
public:																				\
virtual bool Is(const library::rtti::TypeId typeId) const							\
{																					\
	return library::rtti::GetTypeId<Class>() == typeId;								\
}																					\
virtual library::rtti::TypeId GetTypeId() const										\
{																					\
	return library::rtti::GetTypeId<Class>();										\
}																					\
virtual void* CastTo(const library::rtti::TypeId typeId)							\
{																					\
	if (library::rtti::GetTypeId<Class>() == typeId)								\
		return static_cast<Class*>(this);											\
																					\
	return nullptr;																	\
}																					\
virtual const void* CastTo(const library::rtti::TypeId typeId) const				\
{																					\
	if (library::rtti::GetTypeId<Class>() == typeId)								\
		return const_cast<Class*>(static_cast<const Class*>(this));					\
																					\
	return nullptr;																	\
}																					\
template <class T>																	\
T* As()																				\
{																					\
	return static_cast<T*>(CastTo(library::rtti::GetTypeId<T>()));					\
}																					\
template <class T>																	\
const T* As() const																	\
{																					\
	return static_cast<const T*>(CastTo(library::rtti::GetTypeId<T>()));			\
}

//-------------------------------------------------------------------------

#define RTTI_CLASS(Class, ...)														\
public:																				\
library::rtti::TypeId GetTypeId() const override									\
{																					\
	return library::rtti::GetTypeId<Class>();										\
}																					\
bool Is(const library::rtti::TypeId typeId) const override							\
{																					\
	return !!CastTo(typeId);														\
}																					\
void* CastTo(const library::rtti::TypeId typeId) override							\
{																					\
	auto self = static_cast<Class*>(this);											\
	if (library::rtti::GetTypeId<Class>() ==  typeId)								\
		return self;																\
																					\
	return library::rtti::detail::ParentCast<Class, __VA_ARGS__>()(self, typeId);	\
}																					\
const void* CastTo(const library::rtti::TypeId typeId) const override				\
{																					\
	auto self = const_cast<Class*>(static_cast<const Class*>(this));				\
	if (library::rtti::GetTypeId<Class>() == typeId)								\
		return self;																\
																					\
	return library::rtti::detail::ParentCast<Class, __VA_ARGS__>()(self, typeId);	\
}																					\
template <class T>																	\
T* As()																				\
{																					\
	return static_cast<T*>(CastTo(library::rtti::GetTypeId<T>()));					\
}																					\
template <class T>																	\
const T* As() const																	\
{																					\
	return static_cast<const T*>(CastTo(library::rtti::GetTypeId<T>()));			\
}
