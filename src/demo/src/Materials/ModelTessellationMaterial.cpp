#include "Materials/ModelTessellationMaterial.h"

#include <library/Effect/Effect.h>
#include "library/Effect/EffectTechnique.h"

using namespace library;

ModelTessellationMaterial::ModelTessellationMaterial(Effect& effect)
	: TessellationMaterial(effect, "solid_color_manual_factors")

	, m_world(effect.GetVariable("World"))
	, m_cameraPosition(effect.GetVariable("CameraPosition"))
	, m_colorTexture(effect.GetVariable("ColorTexture"))
	, m_maxTessFactor(effect.GetVariable("MaxTessFactor"))
	, m_minTessDistance(effect.GetVariable("MinTessDistance"))
	, m_maxTessDistance(effect.GetVariable("MaxTessDistance"))
{
}

void ModelTessellationMaterial::InitializeInternal()
{
	for (unsigned i = 0, techniquesCount = GetEffect().GetTechniquesCount(); i < techniquesCount; i++)
	{
		auto& technique = GetEffect().GetTechnique(i);

		for (unsigned j = 0, passesCount = technique.GetPassesCount(); j < passesCount; j++)
		{
			auto& pass = technique.GetPass(j);
			CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), pass);
		}
	}
}
