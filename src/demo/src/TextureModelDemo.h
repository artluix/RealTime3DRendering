#pragma once
#include <library/Components/SimplePrimitiveComponent.h>
#include <library/Components/InputReceivableComponent.h>

namespace library
{
	class Mesh;
} // namespace library

class TextureModelDemo
	: public library::SimplePrimitiveComponent
	, public library::InputReceivableComponent
{
	RTTI_CLASS(TextureModelDemo, library::SimplePrimitiveComponent, library::InputReceivableComponent)

public:
	explicit TextureModelDemo();

	void Initialize() override;
	void Update(const library::Time& time) override;

protected:
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;

private:
	void CreateVertexBuffer(const library::ComPtr<ID3D11Device>& device, const library::Mesh& mesh);

	library::ComPtr<ID3DX11EffectMatrixVariable> m_wvpVariable;
	library::ComPtr<ID3DX11EffectShaderResourceVariable> m_colorTextureVariable;

	long m_wheel;
};
