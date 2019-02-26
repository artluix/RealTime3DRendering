#pragma once
#include "library/NonConstructible.hpp"
#include "library/RTTI.hpp"

#include <unordered_map>

namespace library
{
	class ServiceContainer : public NonConstructible<ServiceContainer>
	{
	public:
		static void AddService(const rtti::TypeId typeId, void* const service);
		static void RemoveService(const rtti::TypeId typeId);
		static void* GetService(const rtti::TypeId typeId);

	private:
		static std::unordered_map<rtti::TypeId, void*> s_services;
	};
} // namespace library
