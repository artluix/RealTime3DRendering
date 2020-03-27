#pragma once
#include "Materials/LightsMaterial.h"

class PbrLightingMaterial : public LightsMaterial
{
	RTTI_CLASS(PbrLightingMaterial, LightsMaterial)

public:
	using Vertex = library::VertexPositionTextureNormal;

	explicit PbrLightingMaterial(library::Effect& effect, const std::string& defaultTechniqueName = "forward");

	library::EffectVariable& GetAmbientColor() { return m_ambientColor; }
	library::EffectVariable& GetCameraPosition() { return m_cameraPosition; }

	library::EffectVariable& GetWVP() { return m_wvp; }
	library::EffectVariable& GetWorld() { return m_world; }

private:
	library::EffectVariable& m_ambientColor;
	library::EffectVariable& m_cameraPosition;

	library::EffectVariable& m_wvp;
	library::EffectVariable& m_world;
};
