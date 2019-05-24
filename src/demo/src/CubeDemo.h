#pragma once
#include <library/Components/SimplePrimitiveComponent.h>
#include <library/Components/InputReceivableComponent.h>

class CubeDemo
	: public library::SimplePrimitiveComponent
	, public library::InputReceivableComponent
{
	RTTI_CLASS(CubeDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	CubeDemo() = default;

	void Update(const library::Time& time) override;

protected:
	void InitializeInternal() override;
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;

private:
	library::ComPtr<ID3DX11EffectMatrixVariable> m_wvpVariable;
};
