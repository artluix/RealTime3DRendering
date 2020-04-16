#pragma once
#include "library/Materials/Material.h"

namespace library
{
class IrradianceConvolutionMaterial : public Material
{
	RTTI_CLASS(IrradianceConvolutionMaterial, Material)

public:
	using Vertex = VertexPosition;

	IrradianceConvolutionMaterial(Effect& effect);

	EffectVariable& GetViewProjection() { return m_viewProjection; }
	EffectVariable& GetSampleDelta() { return m_sampleDelta; }
	EffectVariable& GetEnvironmentMap() { return m_environmentMap; }

protected:
	void InitializeInternal() override;

private:
	EffectVariable& m_viewProjection;
	EffectVariable& m_sampleDelta;
	EffectVariable& m_environmentMap;
};
} // namespace library
