#include "StdAfx.h"
#include "library/materials/MaterialFactory.hpp"

#include <memory>
#include <algorithm>

namespace library
{
	std::map<library::rtti::TypeId, Material*> MaterialFactory::s_materials;

	void MaterialFactory::Clear()
	{
		s_materials.clear();
	}

} // namespace library
