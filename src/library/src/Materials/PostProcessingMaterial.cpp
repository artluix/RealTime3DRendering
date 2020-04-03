#include "StdAfx.h"
#include "library/Materials/PostProcessingMaterial.h"

#include "library/Effect/Effect.h"
#include "library/Effect/EffectTechnique.h"
#include "library/Effect/EffectPass.h"

namespace library
{
PostProcessingMaterial::PostProcessingMaterial(Effect& effect)
	: Material(effect)
	, m_sceneTexture(effect.GetVariable("SceneTexture"))
{}

PostProcessingMaterial::~PostProcessingMaterial() = default;

//-------------------------------------------------------------------------

void PostProcessingMaterial::InitializeInternal()
{
	auto descriptionsBuffer = MakeArrayBuffer(Vertex::ElementDescriptions);

	for (unsigned i = 0, techniquesCount = GetEffect().GetTechniquesCount(); i < techniquesCount; i++)
	{
		auto& technique = GetEffect().GetTechnique(i);

		for (unsigned j = 0, passesCount = technique.GetPassesCount(); j < passesCount; j++)
		{
			auto& pass = technique.GetPass(j);
			CreateInputLayout(descriptionsBuffer, pass);
		}
	}
}
} // namespace library
