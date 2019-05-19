#pragma once
#include "library/Materials/Material.h"
#include "library/VertexTypes.h"

namespace library
{
class DiffuseLightingMaterial : public Material
{
	RTTI_CLASS(DiffuseLightingMaterial, Material)

public:
	using Vertex = VertexPositionTextureNormal;

	explicit DiffuseLightingMaterial(std::shared_ptr<Effect> effect);
	~DiffuseLightingMaterial();

	EffectVariable& GetAmbientColor() { return m_ambientColor; }
	EffectVariable& GetLightColor() { return m_lightColor; }
	EffectVariable& GetLightDirection() { return m_lightDirection; }

	EffectVariable& GetWVP() { return m_wvp; }
	EffectVariable& GetWorld() { return m_world; }

	EffectVariable& GetColorTexture() { return m_colorTexture; }

	VertexBufferData CreateVertexBufferData(ID3D11Device* const device, const Mesh& mesh) const override;
	unsigned GetVertexSize() const override { return sizeof(Vertex); }

protected:
	void InitializeInternal() override;

private:
	EffectVariable& m_ambientColor;
	EffectVariable& m_lightColor;
	EffectVariable& m_lightDirection;

	EffectVariable& m_wvp;
	EffectVariable& m_world;

	EffectVariable& m_colorTexture;
};
} // namespace library
