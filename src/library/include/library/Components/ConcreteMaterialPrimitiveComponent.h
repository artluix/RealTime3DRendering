#pragma once
#include "library/Materials/Material.h"
#include "library/Components/PrimitiveComponent.h"

#include "library/Effect/Effect.h"
#include "library/Effect/EffectPass.h"

#include "library/Model/Model.h"
#include "library/Model/Mesh.h"

#include "library/Application.h"

#include <memory>
#include <type_traits>

namespace library
{
template <class MaterialType, typename = std::enable_if_t<std::is_base_of_v<Material, MaterialType>>>
class ConcreteMaterialPrimitiveComponent : public PrimitiveComponent
{
public:
	using Material = MaterialType;

	const Effect& GetEffect() const { return m_effect; }

	Material* GetMaterial() override { return m_material.get(); }
	const Material* GetMaterial() const override { return m_material.get(); }

	ID3D11InputLayout* GetInputLayout() const override { return GetMaterial()->GetCurrentInputLayout(); }

protected:
	ConcreteMaterialPrimitiveComponent() = default;

	void CreateMaterialWithEffect(const std::string& effectName, const bool compile = false)
	{
		m_effect = Effect::Create(GetApp(), effectName, compile);

		m_material = std::make_unique<Material>(*m_effect);
		m_material->Initialize();
	}

	void Draw_SetData(const PrimitiveData& primitiveData) override
	{
		auto& pass = GetMaterial()->GetCurrentPass();
		pass.Apply(0, GetApp().GetDeviceContext());
	}

	void AddPrimitiveData(const Mesh& mesh)
	{
		m_primitivesData.emplace_back(mesh.CreatePrimitiveData<Material::Vertex>());
	}

	void CreatePrimitivesData(const Model& model)
	{
		if (!m_primitivesData.empty())
			m_primitivesData.clear();

		const auto meshesCount = model.GetMeshesCount();
		m_primitivesData.reserve(meshesCount);

		for (unsigned i = 0; i < meshesCount; i++)
		{
			AddPrimitiveData(model.GetMesh(i));
		}
	}

	std::shared_ptr<Effect> m_effect;
	std::unique_ptr<Material> m_material;
};
} // namespace library
