#pragma once
#include "library/Materials/TessellationMaterial.h"

namespace library
{
class ModelTessellationMaterial : public TessellationMaterial
{
	RTTI_CLASS(ModelTessellationMaterial, TessellationMaterial)

public:
	using Vertex = VertexPositionTexture;

	explicit ModelTessellationMaterial(Effect& effect);

	EffectVariable& GetWorld() { return m_world; }
	EffectVariable& GetCameraPosition() { return m_cameraPosition; }
	EffectVariable& GetColorTexture() { return m_colorTexture; }

	EffectVariable& GetMaxTessFactor() { return m_maxTessFactor; }
	EffectVariable& GetMinTessDistance() { return m_minTessDistance; }
	EffectVariable& GetMaxTessDistance() { return m_maxTessDistance; }

	unsigned GetVertexSize() const override { return sizeof(Vertex); }

	VertexBufferData CreateVertexBufferData(ID3D11Device* const device, const Mesh& mesh) const override;

protected:
	void InitializeInternal() override;

	EffectVariable& m_world;
	EffectVariable& m_cameraPosition;
	EffectVariable& m_colorTexture;

	EffectVariable& m_maxTessFactor;
	EffectVariable& m_minTessDistance;
	EffectVariable& m_maxTessDistance;
};
} // namespace library
