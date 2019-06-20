#pragma once
#include <library/Materials/Material.h>

class TessellationMaterial : public library::Material
{
	RTTI_CLASS(TessellationMaterial, library::Material)

public:
	using Vertex = library::VertexPosition;

	explicit TessellationMaterial(library::Effect& effect, const std::string& defaultTechniqueName = "main11");

	library::EffectVariable& GetWVP() { return m_wvp; }
	library::EffectVariable& GetTessellationEdgeFactors() { return m_tessellationEdgeFactors; }
	library::EffectVariable& GetTessellationInsideFactors() { return m_tessellationInsideFactors; }

	library::VertexBufferData CreateVertexBufferData(
		ID3D11Device* const device,
		const library::Mesh& mesh
	) const override;
	unsigned GetVertexSize() const override { return sizeof(Vertex); }

protected:
	void InitializeInternal() override;

	library::EffectVariable& m_wvp;
	library::EffectVariable& m_tessellationEdgeFactors;
	library::EffectVariable& m_tessellationInsideFactors;
};
