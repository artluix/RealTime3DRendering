#include "Materials/PointSpriteMaterial.h"

#include <library/Effect/Effect.h>
#include "library/Effect/EffectTechnique.h"

using namespace library;

PointSpriteMaterial::PointSpriteMaterial(Effect& effect)
	: Material(effect, "main11")

	, m_cameraPosition(effect.GetVariable("CameraPosition"))
	, m_cameraUp(effect.GetVariable("CameraUp"))

	, m_viewProjection(effect.GetVariable("ViewProjection"))

	, m_colorTexture(effect.GetVariable("ColorTexture"))
{}

//-------------------------------------------------------------------------

void PointSpriteMaterial::InitializeInternal()
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
