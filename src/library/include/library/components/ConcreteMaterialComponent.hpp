#pragma once
#include "library/components/MaterialComponent.h"
#include "library/materials/Material.h"

#include <type_traits>

namespace library
{
	class Effect;

	template <class MaterialType, typename = std::enable_if_t<std::is_base_of_v<Material, MaterialType>>>
	class ConcreteMaterialComponent : public MaterialComponent
	{
	public:
		using Material = MaterialType;

	protected:
		using MaterialComponent::MaterialComponent;
		
		std::shared_ptr<Material> m_material;
		std::shared_ptr<Effect> m_effect;
	};

} // namespace library
