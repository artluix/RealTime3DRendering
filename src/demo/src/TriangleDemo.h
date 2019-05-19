#pragma once
#include <library/Components/SceneComponent.h>

#include <library/CommonTypes.h>
#include <library/DirectXForwardDeclarations.h>

class TriangleDemo : public library::SceneComponent
{
	RTTI_CLASS(TriangleDemo, library::SceneComponent)

public:
	explicit TriangleDemo() = default;

	void Initialize() override;
	void Update(const library::Time& time) override;

	unsigned GetVertexSize() const override;

protected:
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;

private:
	library::ComPtr<ID3DX11Effect> m_effect;
	library::ComPtr<ID3DX11EffectTechnique> m_technique;
	library::ComPtr<ID3DX11EffectPass> m_pass;
	library::ComPtr<ID3DX11EffectMatrixVariable> m_wvpVariable;
};
