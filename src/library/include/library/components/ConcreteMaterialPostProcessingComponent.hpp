#pragma once
#include "library/Materials/PostProcessingMaterial.h"
#include "library/Components/PostProcessingComponent.h"

#include "library/Effect/Effect.h"

namespace library
{
	template<
		class MaterialType,
		typename = std::enable_if_t<std::is_base_of_v<PostProcessingMaterial, MaterialType>>
	>
	class ConcreteMaterialPostProcessingComponent : public PostProcessingComponent
	{
	public:
		using Material = MaterialType;

		const Material* GetMaterial() const override { return m_material.get(); }
		const Effect* GetEffect() const override { return m_effect.get(); }

	protected:
		explicit ConcreteMaterialPostProcessingComponent() = default;

		void InitializeMaterial(
			const Application& app,
			const std::string& effectName,
			const bool compile = false
		)
		{
			m_effect = Effect::Create(app, effectName, compile);

			m_material = std::make_unique<Material>(*m_effect);
			m_material->Initialize();
		}

		Material* GetMaterial() override { return m_material.get(); }
		Effect* GetEffect() override { return m_effect.get(); }

		std::shared_ptr<Effect> m_effect;
		std::unique_ptr<Material> m_material;
	};
} // namespace library
