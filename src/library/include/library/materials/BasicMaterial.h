#pragma once
#include "library/Materials/Material.h"

namespace library
{
class BasicMaterial : public Material
{
	RTTI_CLASS(BasicMaterial, Material)

public:
	using Vertex = VertexPositionColor;

	explicit BasicMaterial(Effect& effect);
	~BasicMaterial();

	EffectVariable& GetWorldViewProjection() { return m_worldViewProjection; }

	VertexBufferData CreateVertexBufferData(ID3D11Device* const device, const Mesh& mesh) const override;

protected:
	void InitializeInternal() override;

private:
	EffectVariable& m_worldViewProjection;
};
} // namespace library
