#pragma once
#include <cstdint>
#include <type_traits>

template <class Class, class ...Parents>
class RTTI : public Parents...
{
public:
    static const std::uintptr_t GetStaticTypeId()
    {
        static const auto typeId = reinterpret_cast<std::uintptr_t>(&m_typeId);
        return typeId;
    }

    virtual bool Is(const std::uintptr_t typeId) const
    {
        if constexpr (sizeof...(Parents) == 0)
        {
            return GetStaticTypeId() == typeId;
        }
        else
        {
            return GetStaticTypeId() == typeId || IsParent<Parents...>(typeId);
        }
    }

    template <class T>
    T* As()
    {
        if (Is(T::GetStaticTypeId()))
        {
            if constexpr (std::is_base_of_v<Class, T> || std::is_base_of_v<T, Class>)
            {
                return static_cast<T*>(this);
            }
            else
            {
                return reinterpret_cast<T*>(this);
            }
        }

        return nullptr;
    }

protected:
    template <typename ...Args>
    explicit RTTI(Args&&... args) : Parents(std::forward<Args>(args)...)... {}

    virtual ~RTTI() = default;

private:
    template <class FirstParent>
    bool IsParent(const std::uintptr_t typeId) const
    {
        return FirstParent::Is(typeId);
    }

    template <class FirstParent, class SecondParent, class ...RestParents>
    bool IsParent(const std::uintptr_t typeId) const
    {
        return IsParent<FirstParent>(typeId) || IsParent<SecondParent, RestParents...>(typeId);
    }

    static constexpr std::uintptr_t m_typeId = 0U;
};
