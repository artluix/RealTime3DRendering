#pragma once
#include "library/components/MaterialComponent.h"
#include "library/materials/MaterialFactory.hpp"

namespace library
{
	class Effect;

	template <class MaterialType, typename = std::enable_if_t<std::is_base_of_v<Material, MaterialType>>>
	class ConcreteMaterialComponent : public MaterialComponent
	{
	public:
		const MaterialType& GetMaterial() const override { m_material; }

	protected:
		explicit ConcreteMaterialComponent(const Application& app, const fs::Path& modelPath)
			: DrawableComponent(app)
			, MaterialComponent(app, modelPath)
			, m_effect(effect)
			, m_material(MaterialFactory::Create<MaterialType>(effect))
		{
		}

		MaterialType& GetMaterial() override { m_material; }

		const MaterialType* m_material = nullptr;
		const Effect* m_effect = nullptr;
	};

} // namespace library
