#include "StdAfx.h"
#include "library/Materials/IrradianceConvolutionMaterial.h"

#include "library/Effect/Effect.h"

namespace library
{
IrradianceConvolutionMaterial::IrradianceConvolutionMaterial(Effect& effect)
	: Material(effect, "main11")

	, m_viewProjection(effect.GetVariable("ViewProjection"))
	, m_sampleDelta(effect.GetVariable("SampleDelta"))
	, m_environmentMap(effect.GetVariable("EnvironmentMap"))
{
}

void IrradianceConvolutionMaterial::InitializeInternal()
{
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), GetDefaultTechniqueName());
}

} // namespace library


