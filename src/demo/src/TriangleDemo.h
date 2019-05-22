#pragma once
#include <library/Components/SimplePrimitiveComponent.h>

class TriangleDemo : public library::SimplePrimitiveComponent
{
	RTTI_CLASS(TriangleDemo, library::SimplePrimitiveComponent)

public:
	explicit TriangleDemo() = default;

	void Initialize() override;
	void Update(const library::Time& time) override;

protected:
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;

private:
	library::ComPtr<ID3DX11EffectMatrixVariable> m_wvpVariable;
};
