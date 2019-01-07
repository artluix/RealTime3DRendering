#include "StdAfx.h"
#include "library/materials/MaterialFactory.hpp"

#include <memory>
#include <algorithm>

namespace library
{
	std::map<library::rtti::TypeId, Material*> MaterialFactory::s_materials;

	void MaterialFactory::Clear()
	{
		std::for_each(s_materials.begin(), s_materials.end(), std::default_delete<Material>());
		s_materials.clear();
	}

} // namespace library
