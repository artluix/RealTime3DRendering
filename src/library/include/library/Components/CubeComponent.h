#pragma once
#include "library/Components/PrimitiveComponent.h"
#include "library/Materials/MaterialUpdateFunction.h"

namespace library
{
class EffectPass;

class CubeComponent : public PrimitiveComponent
{
	RTTI_CLASS(CubeComponent, PrimitiveComponent)

public:
	CubeComponent();
	~CubeComponent();

	void SetMaterial(Material& material);
	void SetMaterial(Material& material, const std::string& techniqueName, const std::string& passName = "p0");

	void SetActiveTechnique(const std::string& techniqueName, const std::string& passName = "p0");
	void SetMaterialUpdateFunction(const MaterialUpdateFunction& func);

	void Update(const Time& time) override;

	const Material* GetMaterial() const override { return m_material; }
	Material* GetMaterial() override { return m_material; }

	ID3D11InputLayout* GetInputLayout() const override { return m_inputLayout; }

	void SetVertexBufferData(VertexBufferData&& vbd);

protected:
	void InitializeInternal() override;
	void Draw_SetData(const PrimitiveData& primitiveData) override;

	//Material* GetMaterial() override { return m_material; }

	MaterialUpdateFunction m_materialUpdateFunction;
	Material* m_material = nullptr;

	EffectPass* m_pass = nullptr;
	ID3D11InputLayout* m_inputLayout = nullptr;
};
} // namespace library


