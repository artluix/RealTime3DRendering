#pragma once
#include "library/materials/PostProcessingMaterial.h"
#include "library/components/PostProcessingComponent.h"

#include "library/effect/Effect.h"

namespace library
{
	template<class MaterialType, typename = std::enable_if_t<std::is_base_of_v<PostProcessingMaterial, MaterialType>>>
	class ConcreteMaterialPostProcessingComponent : public PostProcessingComponent
	{
	public:
		using Material = MaterialType;

		const Material* GetMaterial() const override { return m_material.get(); }

	protected:
		explicit ConcreteMaterialPostProcessingComponent() = default;

		void InitializeMaterial(const Application& app, const std::string& effectName, const bool compile = false)
		{
			m_effect = Effect::Create(app, effectName, compile);

			m_material = std::make_unique<Material>(*m_effect);
			m_material->Initialize();
		}

		Material* GetMaterial() override { return m_material.get(); }

		std::shared_ptr<Effect> m_effect;
		std::unique_ptr<Material> m_material;
	};
} // namespace library
