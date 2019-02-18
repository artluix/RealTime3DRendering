#pragma once
#include <library/components/SceneComponent.h>
#include <library/components/DrawableInputComponent.h>
#include <library/components/InputReceivableComponent.h>

#include <library/CommonTypes.h>
#include <library/DirectXForwardDeclarations.h>

namespace library
{
	class Mesh;
	class KeyboardComponent;
	class MouseComponent;
} // namespace library

class TextureModelDemo
	: public library::SceneComponent
	, public library::DrawableInputComponent
	, public library::InputReceivableComponent
{
	RTTI_CLASS(TextureModelDemo, library::SceneComponent, library::DrawableInputComponent, library::InputReceivableComponent)

public:
	explicit TextureModelDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;

	unsigned GetVertexSize() const override;

protected:
	void Draw_SetData() override;

private:
	void CreateVertexBuffer(const ComPtr<ID3D11Device>& device, const library::Mesh& mesh);

	ComPtr<ID3DX11Effect> m_effect;
	ComPtr<ID3DX11EffectTechnique> m_technique;
	ComPtr<ID3DX11EffectPass> m_pass;
	ComPtr<ID3DX11EffectMatrixVariable> m_wvpVariable;

	// texture mapping specific
	ComPtr<ID3DX11EffectShaderResourceVariable> m_colorTextureVariable;

	long m_wheel;
};