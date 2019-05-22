#pragma once
#include <library/Components/SimplePrimitiveComponent.h>
#include <library/Components/InputReceivableComponent.h>

namespace library
{
	class Mesh;
} // namespace library

class ModelDemo
	: public library::SimplePrimitiveComponent
	, public library::InputReceivableComponent
{
	RTTI_CLASS(ModelDemo, library::SimplePrimitiveComponent, library::InputReceivableComponent)

public:
	explicit ModelDemo() = default;

	void Initialize() override;
	void Update(const library::Time& time) override;

protected:
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;

private:
	void CreatePrimitivesData(ID3D11Device* const device, const library::Mesh& mesh);

	library::ComPtr<ID3DX11EffectMatrixVariable> m_wvpVariable;
};
