#pragma once
#include "library/materials/Material.h"
#include "library/components/SceneComponent.h"
#include "library/effect/Effect.h"

#include <type_traits>

namespace library
{
	template<class MaterialType, typename = std::enable_if_t<std::is_base_of_v<Material, MaterialType>>>
	class ConcreteMaterialSceneComponent : public SceneComponent
	{
	public:
		using Material = MaterialType;

		const Material* GetMaterial() const override { return m_material.get(); }

	protected:
		explicit ConcreteMaterialSceneComponent() = default;

		void InitializeMaterial(const Application& app, const std::string& effectName, const bool compile = false)
		{
			m_effect = Effect::Create(app, effectName, compile);

			m_material = std::make_unique<Material>(*m_effect);
			m_material->Initialize();
		}

		Material* GetMaterial() override { return m_material.get(); }
		unsigned GetVertexSize() const override { return GetMaterial()->GetVertexSize(); }

		std::shared_ptr<Effect> m_effect;
		std::unique_ptr<Material> m_material;
	};
} // namespace library
