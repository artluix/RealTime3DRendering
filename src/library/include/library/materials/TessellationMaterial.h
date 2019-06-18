#pragma once
#include "library/Materials/Material.h"
#include "library/VertexTypes.h"

namespace library
{
class TessellationMaterial : public Material
{
	RTTI_CLASS(TessellationMaterial, Material)

public:
	using Vertex = VertexPosition;

	explicit TessellationMaterial(Effect& effect, const std::string& defaultTechniqueName = "main11");

	EffectVariable& GetWVP() { return m_wvp; }
	EffectVariable& GetTessellationEdgeFactors() { return m_tessellationEdgeFactors; }
	EffectVariable& GetTessellationInsideFactors() { return m_tessellationInsideFactors; }

	unsigned GetVertexSize() const override { return sizeof(Vertex); }

	VertexBufferData CreateVertexBufferData(ID3D11Device* const device, const Mesh& mesh) const override;

protected:
	void InitializeInternal() override;

	EffectVariable& m_wvp;
	EffectVariable& m_tessellationEdgeFactors;
	EffectVariable& m_tessellationInsideFactors;
};
} // namespace library
