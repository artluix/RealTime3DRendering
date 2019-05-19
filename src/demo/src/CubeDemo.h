#pragma once
#include <library/Components/SceneComponent.h>
#include <library/Components/InputReceivableComponent.h>

#include <library/CommonTypes.h>
#include <library/DirectXForwardDeclarations.h>

namespace library
{
	class KeyboardComponent;
} // namespace library

class CubeDemo
	: public library::SceneComponent
	, public library::InputReceivableComponent
{
	RTTI_CLASS(CubeDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	explicit CubeDemo() = default;

	void Initialize() override;
	void Update(const library::Time& time) override;

protected:
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;

	unsigned GetVertexSize() const override;

	ID3D11InputLayout* GetInputLayout() override { return m_inputLayout.Get(); }

private:
	library::ComPtr<ID3DX11Effect> m_effect;
	library::ComPtr<ID3DX11EffectTechnique> m_technique;
	library::ComPtr<ID3DX11EffectPass> m_pass;
	library::ComPtr<ID3D11InputLayout> m_inputLayout;
	library::ComPtr<ID3DX11EffectMatrixVariable> m_wvpVariable;
};
