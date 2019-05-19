#pragma once
#include <library/Components/SceneComponent.h>
#include <library/Components/InputReceivableComponent.h>

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
	, public library::InputReceivableComponent
{
	RTTI_CLASS(TextureModelDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	explicit TextureModelDemo();

	void Initialize() override;
	void Update(const library::Time& time) override;

	unsigned GetVertexSize() const override;

protected:
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;

private:
	void CreateVertexBuffer(const library::ComPtr<ID3D11Device>& device, const library::Mesh& mesh);

	library::ComPtr<ID3DX11Effect> m_effect;
	library::ComPtr<ID3DX11EffectTechnique> m_technique;
	library::ComPtr<ID3DX11EffectPass> m_pass;
	library::ComPtr<ID3DX11EffectMatrixVariable> m_wvpVariable;

	library::ComPtr<ID3DX11EffectShaderResourceVariable> m_colorTextureVariable;

	long m_wheel;
};
