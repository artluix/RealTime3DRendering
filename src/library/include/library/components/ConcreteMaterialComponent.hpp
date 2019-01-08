#pragma once
#include "library/components/MaterialComponent.h"
#include "library/materials/Material.h"

#include <type_traits>
#include <memory>

namespace library
{
	class Effect;

	template <class MaterialType, typename = std::enable_if_t<std::is_base_of_v<Material, MaterialType>>>
	class ConcreteMaterialComponent : public MaterialComponent
	{
	public:
		using Material = MaterialType;

		const Material& GetMaterial() const { return *m_material; }

	protected:
		using MaterialComponent::MaterialComponent;

		Material& GetMaterial() { return *m_material; }
		
		std::unique_ptr<Material> m_material;
		std::shared_ptr<Effect> m_effect;
	};

} // namespace library
