#pragma once
#include "library/NonCopyable.hpp"
#include "library/RTTI.hpp"

#include <unordered_map>

namespace library
{
    class ServiceContainer : public NonCopyable<ServiceContainer>
    {
    public:
        explicit ServiceContainer() = default;
        ~ServiceContainer() = default;

        void AddService(const rtti::TypeId typeId, void* const service);
        void RemoveService(const rtti::TypeId typeId);
        void* const GetService(const rtti::TypeId typeId) const;

    private:
        std::unordered_map<rtti::TypeId, void* const> m_services;
    };
}

