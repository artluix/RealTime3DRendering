#include "library/ServiceContainer.h"

namespace library
{
    void ServiceContainer::AddService(const rtti::TypeId typeId, void* const service)
    {
        if (m_services.find(typeId) == m_services.end())
        {
            m_services.emplace(std::make_pair(typeId, service));
        }
    }

    void ServiceContainer::RemoveService(const rtti::TypeId typeId)
    {
        if (m_services.find(typeId) != m_services.end())
        {
            m_services.erase(typeId);
        }
    }

    void* const ServiceContainer::GetService(const rtti::TypeId typeId) const
    {
        if (m_services.empty())
            return nullptr;

        auto it = m_services.find(typeId);
        if (it != m_services.end())
            return it->second;

        return nullptr;
    }
}
