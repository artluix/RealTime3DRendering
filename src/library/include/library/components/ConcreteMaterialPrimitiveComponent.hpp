#pragma once
#include "library/Materials/Material.h"
#include "library/Components/PrimitiveComponent.h"

#include "library/Effect/Effect.h"
#include "library/Effect/EffectTechnique.h"
#include "library/Effect/EffectPass.h"

#include <memory>
#include <type_traits>

namespace library
{
template <class MaterialType, typename = std::enable_if_t<std::is_base_of_v<Material, MaterialType>>>
class ConcreteMaterialPrimitiveComponent : public PrimitiveComponent
{
public:
	using Material = MaterialType;

	const Material* GetMaterial() const override { return m_material.get(); }
	ID3D11InputLayout* GetInputLayout() const override { return m_inputLayout; }

protected:
	ConcreteMaterialPrimitiveComponent() = default;

	void InitializeMaterial(const std::string& effectName, const bool compile = false)
	{
		m_material = std::make_unique<Material>(Effect::Create(GetApp(), effectName, compile));
		m_material->Initialize();

		// set default pass and input layout from material
		m_technique = &m_material->GetCurrentTechnique();
		m_pass = &m_technique->GetPass(0);
		m_inputLayout = m_material->GetInputLayout(*m_pass);
	}

	void Draw_SetData(const PrimitiveData& primitiveData) override
	{
		m_pass->Apply(0, GetApp().GetDeviceContext());
	}

	Material* GetMaterial() override { return m_material.get(); }

	std::unique_ptr<Material> m_material;

	EffectPass* m_pass = nullptr;
	EffectTechnique* m_technique = nullptr;
	ID3D11InputLayout* m_inputLayout = nullptr;
};
} // namespace library