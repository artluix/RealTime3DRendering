#pragma once
#include <library/components/SceneComponent.h>
#include <library/components/DrawableInputComponent.h>

#include <library/CommonTypes.h>
#include <library/DirectXForwardDeclarations.h>

class TriangleDemo
	: public library::SceneComponent
	, public library::DrawableInputComponent
{
	RTTI_CLASS(TriangleDemo, library::SceneComponent, library::DrawableInputComponent)

public:
	explicit TriangleDemo() = default;

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;

	unsigned GetVertexSize() const override;

protected:
	void Draw_SetData() override;

private:
	ComPtr<ID3DX11Effect> m_effect;
	ComPtr<ID3DX11EffectTechnique> m_technique;
	ComPtr<ID3DX11EffectPass> m_pass;
	ComPtr<ID3DX11EffectMatrixVariable> m_wvpVariable;
};
