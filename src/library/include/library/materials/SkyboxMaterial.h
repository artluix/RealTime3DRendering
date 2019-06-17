#pragma once
#include "library/Materials/Material.h"
#include "library/VertexTypes.h"

namespace library
{
class SkyboxMaterial : public Material
{
	RTTI_CLASS(SkyboxMaterial, Material)

public:
	using Vertex = VertexPosition;

	explicit SkyboxMaterial(Effect& effect);
	~SkyboxMaterial();

	EffectVariable& GetWorldViewProjection() { return m_worldViewProjection; }
	EffectVariable& GetSkyboxTexture() { return m_skyboxTexture; }

	VertexBufferData CreateVertexBufferData(ID3D11Device* const device, const Mesh& mesh) const override;
	unsigned GetVertexSize() const override { return sizeof(Vertex); }

protected:
	void InitializeInternal() override;

private:
	EffectVariable& m_worldViewProjection;
	EffectVariable& m_skyboxTexture;
};
} // namespace library
