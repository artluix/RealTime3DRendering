#pragma once
#include "library/Materials/PostProcessingMaterial.h"
#include "library/Components/PostProcessingComponent.h"
#include "library/Effect/Effect.h"

namespace library
{
template <
	class MaterialType,
	typename = std::enable_if_t<std::is_base_of_v<PostProcessingMaterial, MaterialType>>>
class ConcreteMaterialPostProcessingComponent : public PostProcessingComponent
{
public:
	using Material = MaterialType;

	const Material* GetMaterial() const override { return m_material.get(); }

protected:
	ConcreteMaterialPostProcessingComponent() = default;

	void InitializeMaterial(const std::string& effectName, const bool compile = false)
	{
		m_material = std::make_unique<Material>(Effect::Create(GetApp(), effectName, compile));
		m_material->Initialize();
	}

	Material* GetMaterial() override { return m_material.get(); }

	std::unique_ptr<Material> m_material;
};
} // namespace library
