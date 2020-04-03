#pragma once
#include "Materials/LightsMaterial.h"

class PhongLightingMaterial : public LightsMaterial
{
	RTTI_CLASS(PhongLightingMaterial, LightsMaterial)

public:
	library::EffectVariable& GetAmbientColor() { return m_ambientColor; }

	library::EffectVariable& GetSpecularPower() { return m_specularPower; }
	library::EffectVariable& GetSpecularColor() { return m_specularColor; }

	// deferred part
	library::EffectVariable& GetColorBufferTexture() { return m_colorBufferTexture; }
	library::EffectVariable& GetNormalBufferTexture() { return m_normalBufferTexture; }
	library::EffectVariable& GetWorldPositionBufferTexture() { return m_worldPositionBufferTexture; }
	library::EffectVariable& GetDepthBufferTexture() { return m_depthBufferTexture; }

protected:
	explicit PhongLightingMaterial(library::Effect& effect, const std::string& defaultTechniqueName);

private:
	library::EffectVariable& m_ambientColor;

	library::EffectVariable& m_specularPower;
	library::EffectVariable& m_specularColor;

	// for deferred shading
	library::EffectVariable& m_colorBufferTexture;
	library::EffectVariable& m_normalBufferTexture;
	library::EffectVariable& m_worldPositionBufferTexture;
	library::EffectVariable& m_depthBufferTexture;
};
