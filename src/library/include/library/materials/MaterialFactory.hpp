#pragma once
#include "library/NonConstructible.hpp"
#include "library/RTTI.hpp"
#include "library/materials/Material.h"

#include <map>
#include <type_traits>
#include <memory>

namespace library
{
	class MaterialFactory : public NonConstructible<MaterialFactory>
	{
	public:
		template<class T, typename = std::enable_if_t<std::is_base_of_v<Material, T>>>
		static std::shared_ptr<T> Create()
		{
			const auto typeId = rtti::GetTypeId<T>();

			auto it = s_materials.find(typeId);
			if (it != s_materials.end())
			{
				return rtti::CastTo<T>(it->second);
			}

			auto material = std::make_shared<T>();
			s_materials.emplace(typeId, material);
			return *material;
		}

		static void Clear();

	private:
		static std::map<rtti::TypeId, MaterialPtr> s_materials;
	};

} // namespace library
