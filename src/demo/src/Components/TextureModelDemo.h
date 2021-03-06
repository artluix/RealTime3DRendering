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
	RTTI_CLASS(TextureModelDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	TextureModelDemo();

	void Update(const library::Time& time) override;

protected:
	void InitializeInternal() override;
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;

private:
	struct Texture
	{
		enum Type : unsigned
		{
			Default = 0,

			//# Count
			Count
		};
	};

	library::ComPtr<ID3DX11EffectMatrixVariable> m_wvpVariable;
	library::ComPtr<ID3DX11EffectShaderResourceVariable> m_colorTextureVariable;

	long m_wheel;
};
