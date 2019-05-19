#include "StdAfx.h"
#include "library/ServiceContainer.h"

namespace library
{
std::unordered_map<library::rtti::TypeId, void*> ServiceContainer::s_services;

void ServiceContainer::AddService(const rtti::TypeId typeId, void* const service)
{
	if (s_services.find(typeId) == s_services.end())
	{
		s_services.emplace(typeId, service);
	}
}

void ServiceContainer::RemoveService(const rtti::TypeId typeId)
{
	if (s_services.find(typeId) != s_services.end())
	{
		s_services.erase(typeId);
	}
}

void* ServiceContainer::GetService(const rtti::TypeId typeId)
{
	if (s_services.empty())
		return nullptr;

	auto it = s_services.find(typeId);
	if (it != s_services.end())
		return it->second;

	return nullptr;
}
} // namespace library
