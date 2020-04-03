#include "Materials/TessellationMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

TessellationMaterial::TessellationMaterial(
	Effect& effect,
	const std::string& defaultTechniqueName /* = "main11" */
)
	: Material(effect, defaultTechniqueName)

	, m_wvp(effect.GetVariable("WVP"))
	, m_tessellationEdgeFactors(effect.GetVariable("TessellationEdgeFactors"))
	, m_tessellationInsideFactors(effect.GetVariable("TessellationInsideFactor"))
{}

//-------------------------------------------------------------------------

void TessellationMaterial::InitializeInternal()
{
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), GetDefaultTechniqueName());
}
