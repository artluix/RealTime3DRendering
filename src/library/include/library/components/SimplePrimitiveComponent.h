#pragma once
#include "library/Components/PrimitiveComponent.h"
#include "library/FxForward.h"

namespace library
{
class SimplePrimitiveComponent : public PrimitiveComponent
{
	RTTI_CLASS(SimplePrimitiveComponent, PrimitiveComponent)

public:
	SimplePrimitiveComponent();
	~SimplePrimitiveComponent();

	ID3D11InputLayout* GetInputLayout() const override { return m_inputLayout.Get(); }

protected:
	void InitializeEffect(const std::string& effectName, const bool compile = false);

	void Draw_SetData(const PrimitiveData& primitiveData) override;

	ComPtr<ID3DX11Effect> m_effect;
	ComPtr<ID3DX11EffectTechnique> m_technique;
	ComPtr<ID3DX11EffectPass> m_pass;
	ComPtr<ID3D11InputLayout> m_inputLayout;
};
} // namespace library
