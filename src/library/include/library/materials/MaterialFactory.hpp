#pragma once
#include "library/NonConstructible.hpp"
#include "library/RTTI.hpp"
#include "library/materials/Material.h"

#include <map>
#include <type_traits>

namespace library
{
	class MaterialFactory : public NonConstructible<MaterialFactory>
	{
	public:
		template<class T, typename = std::enable_if_t<std::is_base_of_v<Material, T>>>
		static T& Create(const Effect& effect)
		{
			const auto typeId = rtti::GetTypeId<T>();

			auto it = s_materials.find(typeId);
			if (it != s_materials.end())
			{
				auto material = it->second;
				return *material->As<T>();
			}

			auto material = new T(effect);
			s_materials.emplace(typeId, material);
			return *material;
		}

		static void Clear();

	private:
		static std::map<rtti::TypeId, Material*> s_materials;
	};

} // namespace library
