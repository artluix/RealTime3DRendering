#pragma once
#include "library/Materials/Material.h"
#include "library/VertexTypes.h"

namespace library
{
class EnvironmentMappingMaterial : public Material
{
	RTTI_CLASS(EnvironmentMappingMaterial, Material)

public:
	using Vertex = VertexPositionTextureNormal;

	explicit EnvironmentMappingMaterial(Effect& effect);
	~EnvironmentMappingMaterial();

	EffectVariable& GetAmbientColor() { return m_ambientColor; }
	EffectVariable& GetEnvironmentColor() { return m_environmentColor; }
	EffectVariable& GetCameraPosition() { return m_cameraPosition; }

	EffectVariable& GetWVP() { return m_wvp; }
	EffectVariable& GetWorld() { return m_world; }
	EffectVariable& GetReflectionAmount() { return m_reflectionAmount; }

	EffectVariable& GetColorTexture() { return m_colorTexture; }
	EffectVariable& GetEnvironmentMap() { return m_environmentMap; }

	VertexBufferData CreateVertexBufferData(ID3D11Device* const device, const Mesh& mesh) const override;
	unsigned GetVertexSize() const override { return sizeof(Vertex); }

protected:
	void InitializeInternal() override;

private:
	EffectVariable& m_ambientColor;
	EffectVariable& m_environmentColor;
	EffectVariable& m_cameraPosition;

	EffectVariable& m_wvp;
	EffectVariable& m_world;
	EffectVariable& m_reflectionAmount;

	EffectVariable& m_colorTexture;
	EffectVariable& m_environmentMap;
};
} // namespace library
